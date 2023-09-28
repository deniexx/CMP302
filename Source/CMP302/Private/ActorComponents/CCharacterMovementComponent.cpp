// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CCharacterMovementComponent.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CDashUserWidget.h"

UCCharacterMovementComponent::UCCharacterMovementComponent()
{
	bDashing = false;
	bDashConsumed = false;
	DashTimeSlowDuration = 1.5f;
	DefaultAirControl = 0.05f;
	DashDistance = 750.f;
	DefaultMaxWalkSpeed = 600;
	DefaultMaxAcceleration = 2048;
}

void UCCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	DefaultAirControl = AirControl;
	DefaultMaxWalkSpeed = MaxWalkSpeed;
	DefaultMaxAcceleration = GetMaxAcceleration();
}

void UCCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDashing && DashIndicatorActor)
	{
		ACharacter* Owner = GetCharacterOwner();
		FVector TraceStart = Owner->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (Owner->GetControlRotation().Vector() * DashDistance);
		FVector IndicatorLocation = TraceEnd;
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		const float ScaledCapsuleHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
		if (bHit)
		{
			TraceEnd = HitResult.ImpactPoint;
			/** Do one more line trace in case to check for the bottom location of the Indicator */
			FVector Down = TraceEnd;
			FCollisionShape Shape;
			Shape.SetCapsule(CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius(), CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			Down.Z -= 10000000.f;
			if (GetWorld()->SweepSingleByChannel(HitResult, TraceEnd, Down, FQuat::Identity, ECC_Visibility, Shape, QueryParams))
			{
				IndicatorLocation = HitResult.ImpactPoint;
				DashLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, ScaledCapsuleHalfHeight);
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

void UCCharacterMovementComponent::BeginDash()
{
	if (bDashConsumed || bDashing) return;
	if (DashTimerHandle.IsValid()) return;
	
	bDashing = true;
	AirControl = 1.f;
	MaxWalkSpeed = 2 * MaxWalkSpeed;
	MaxAcceleration = 2 * MaxAcceleration;
	UGameplayStatics::SetGlobalTimeDilation(GetOwner(), 0.2f);

	if(!DashIndicatorActor)
	{
		ensureAlways(DashIndicatorMesh);
		DashIndicatorActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
		DashIndicatorActor->SetMobility(EComponentMobility::Movable);
		DashIndicatorActor->GetStaticMeshComponent()->SetStaticMesh(DashIndicatorMesh);
	}

	if (!DashVisualOverlayInstance)
	{
		ensureAlways(DashVisualOverlayInstanceClass);
		DashVisualOverlayInstance = CreateWidget<UCDashUserWidget>(CharacterOwner->GetController<APlayerController>(), DashVisualOverlayInstanceClass);
		DashVisualOverlayInstance->BindMovementComponent(this);
	}

	DashVisualOverlayInstance->AddToViewport();
	DashIndicatorActor->SetActorHiddenInGame(false);
	
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UCCharacterMovementComponent::InterruptDash, DashTimeSlowDuration);
}

void UCCharacterMovementComponent::Dash()
{
	GetOwner()->SetActorLocation(DashLocation, true, nullptr, ETeleportType::ResetPhysics);
}

void UCCharacterMovementComponent::ResetDash()
{
	AirControl = DefaultAirControl;
	MaxWalkSpeed = DefaultMaxWalkSpeed;
	MaxAcceleration = DefaultMaxAcceleration;
	bDashing = false;
	
	if (DashIndicatorActor)
		DashIndicatorActor->SetActorHiddenInGame(true);

	if (DashVisualOverlayInstance)
		DashVisualOverlayInstance->RemoveFromParent();
	
	UGameplayStatics::SetGlobalTimeDilation(GetOwner(), 1.f);

	if (DashTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
	}

	DashTimerHandle.Invalidate();
}

void UCCharacterMovementComponent::EndDash()
{
	bDashConsumed = true;
	if (bDashing)
		Dash();
	
	ResetDash();
}

void UCCharacterMovementComponent::InterruptDash()
{
	RefillDash();
}

void UCCharacterMovementComponent::RefillDash()
{
	ResetDash();
	bDashConsumed = false;
}

float UCCharacterMovementComponent::GetDashTimerProgress() const
{
	const float Remaining = GetWorld()->GetTimerManager().GetTimerRemaining(DashTimerHandle);
	return Remaining / DashTimeSlowDuration;
}

void UCCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Falling)
	{
		RefillDash();
	}
}
