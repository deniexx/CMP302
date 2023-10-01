// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_Dash.h"

#include "ActorComponents/CExtendedCharacterMovement.h"
#include "Character/CCommonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CDashUserWidget.h"

void UCAction_Dash::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Character = Cast<ACCommonCharacter>(InInstigator);
	MovementComponent = Character->GetCharacterMovement<UCExtendedCharacterMovement>();
	
	DefaultAirControl = MovementComponent->AirControl;
	DefaultMaxWalkSpeed = MovementComponent->MaxWalkSpeed;
	DefaultMaxAcceleration = MovementComponent->GetMaxAcceleration();

	MovementComponent->OnMovementChanged.AddDynamic(this, &UCAction_Dash::OnMovementModeChanged);
}

void UCAction_Dash::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);
}

void UCAction_Dash::TickAction_Implementation(float DeltaTime)
{
	Super::TickAction_Implementation(DeltaTime);

	if (bDashing && DashIndicatorActor)
	{
		FHitResult DashHitResult;
		ACharacter* Owner = Character;
		FVector TraceStart = Owner->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (Owner->GetControlRotation().Vector() * DashDistance);
		FVector IndicatorLocation = TraceEnd;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Character);

		const float ScaledCapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		bool bHit = GetWorld()->LineTraceSingleByChannel(DashHitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
		if (bHit)
		{
			TraceEnd = DashHitResult.ImpactPoint;
			/** Do one more line trace in case to check for the bottom location of the Indicator */
			FVector Down = TraceEnd;
			FCollisionShape Shape;
			Shape.SetCapsule(Character->GetCapsuleComponent()->GetScaledCapsuleRadius(), Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			Down.Z -= 10000000.f;
			if (GetWorld()->SweepSingleByChannel(DashHitResult, TraceEnd, Down, FQuat::Identity, ECC_Visibility, Shape, QueryParams))
			{
				IndicatorLocation = DashHitResult.ImpactPoint;
				DashLocation = DashHitResult.ImpactPoint + FVector(0.f, 0.f, ScaledCapsuleHalfHeight);
			}
		}
		else
		{
			DashLocation = TraceEnd;
			IndicatorLocation.Z -= ScaledCapsuleHalfHeight;
		}

		FRotator IndicatorRotation = Owner->GetActorForwardVector().Rotation() + FRotator(0.0f, -90.f, 0.f);
		DashIndicatorActor->SetActorLocationAndRotation(IndicatorLocation, IndicatorRotation);
	}
}

void UCAction_Dash::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	if (DashTimerHandle.IsValid())
	{
		bDashing = false;
		StopAction(InInstigator);
		return;
	}

	bDashing = true;
	MovementComponent->AirControl = 1.f;
	MovementComponent->MaxWalkSpeed = 2 * MovementComponent->MaxWalkSpeed;
	MovementComponent->MaxAcceleration = 2 * MovementComponent->MaxAcceleration;
	UGameplayStatics::SetGlobalTimeDilation(GetOuter(), 0.2f);

	if (!DashIndicatorActor)
	{
		ensureAlways(DashIndicatorMesh);
		DashIndicatorActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
		DashIndicatorActor->SetMobility(EComponentMobility::Movable);
		DashIndicatorActor->GetStaticMeshComponent()->SetStaticMesh(DashIndicatorMesh);
	}

	if (!DashVisualOverlayInstance)
	{
		ensureAlways(DashVisualOverlayInstanceClass);
		DashVisualOverlayInstance = CreateWidget<UCDashUserWidget>(Character->GetController<APlayerController>(), DashVisualOverlayInstanceClass);
		DashVisualOverlayInstance->BindDashAction(this);
	}

	DashVisualOverlayInstance->AddToViewport();
	DashIndicatorActor->SetActorHiddenInGame(false);

	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UCAction_Dash::InterruptDash, DashTimeSlowDuration);
}

void UCAction_Dash::StopAction_Implementation(AActor* InInstigator)
{
	Super::StopAction_Implementation(InInstigator);
	
	if (bDashing)
	{
		Dash();
		ResetDash();
	}
}

bool UCAction_Dash::CanStart_Implementation(AActor* InInstigator)
{
	return Super::CanStart_Implementation(InInstigator) && !bDashing;
}

void UCAction_Dash::InterruptDash()
{
	ResetDash();
	bDashing = false;

	if (MovementComponent->IsMovingOnGround())
	{
		StopAction(Character);
	}
}

void UCAction_Dash::Dash()
{
	Character->SetActorLocation(DashLocation, false, nullptr, ETeleportType::ResetPhysics);
	const FVector VelocityDirection = Character->GetActorForwardVector();
	MovementComponent->Velocity = VelocityDirection * VelocityAfterDash;

	if (MovementComponent->IsMovingOnGround())
	{
		bDashing = false;
		StopAction(Character);
	}
}

void UCAction_Dash::ResetDash()
{
	MovementComponent->AirControl = DefaultAirControl;
	MovementComponent->MaxWalkSpeed = DefaultMaxWalkSpeed;
	MovementComponent->MaxAcceleration = DefaultMaxAcceleration;
	
	if (DashIndicatorActor)
		DashIndicatorActor->SetActorHiddenInGame(true);

	if (DashVisualOverlayInstance)
		DashVisualOverlayInstance->RemoveFromParent();
	
	UGameplayStatics::SetGlobalTimeDilation(GetOuter(), 1.f);

	if (DashTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
	}

	DashTimerHandle.Invalidate();
}

float UCAction_Dash::GetDashTimerProgress() const
{
	const float Remaining = GetWorld()->GetTimerManager().GetTimerRemaining(DashTimerHandle);
	return Remaining / DashTimeSlowDuration;
}

void UCAction_Dash::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMovementMode)
{
	if (PreviousMovementMode == MOVE_Falling)
	{
		bDashing = false;
		ResetDash();
	}
}
