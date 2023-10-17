// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_Dash.h"

#include "ActorComponents/CActionComponent.h"
#include "ActorComponents/CCombatStatusComponent.h"
#include "ActorComponents/CExtendedCharacterMovement.h"
#include "Character/CCommonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "System/CGameplayFunctionLibrary.h"
#include "UI/CDashUserWidget.h"

void UCAction_Dash::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Character = Cast<ACCommonCharacter>(InInstigator);
	MovementComponent = Character->GetCharacterMovement<UCExtendedCharacterMovement>();
	
	DefaultAirControl = MovementComponent->AirControl;
	DefaultMaxWalkSpeed = MovementComponent->MaxWalkSpeed;
	DefaultMaxAcceleration = MovementComponent->GetMaxAcceleration();
	bGroundTouched = true;
	
	if (ensure(DashCooldownWidgetClass))
	{
		APlayerController* PlayerController = Character->GetController<APlayerController>();
		DashCooldownWidgetInstance = CreateWidget(PlayerController, DashCooldownWidgetClass);
		DashCooldownWidgetInstance->AddToViewport();
		UKismetMaterialLibrary::SetScalarParameterValue(Character, UIMaterialParameters, TEXT("DashCooldownProgress"), 1);
	}

	MovementComponent->OnMovementChanged.AddDynamic(this, &UCAction_Dash::OnMovementModeChanged);
}

void UCAction_Dash::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);

	if (DashCooldownWidgetInstance)
		DashCooldownWidgetInstance->RemoveFromParent();
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
	
	bDashing = true;
	bGroundTouched = false;
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

	const FString Message = FString::Printf(TEXT("%s engaged."), *ActionName.ToString());
	UCGameplayFunctionLibrary::AddStatusReportMessage(GetOuter(), Message);
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
	return Super::CanStart_Implementation(InInstigator) && !bDashing && bGroundTouched;
}

void UCAction_Dash::InterruptDash()
{
	ResetDash();
	bDashing = false;

	if (MovementComponent->IsMovingOnGround())
	{
		StopAction(Character);
		bGroundTouched = true;
	}

	const FString Message = FString::Printf(TEXT("%s malfunctioned."), *ActionName.ToString());
	UCGameplayFunctionLibrary::AddStatusReportMessage(GetOuter(), Message);
}

void UCAction_Dash::Dash()
{
	bDashing = false;

	FTweenHandle Handle;
	FTweenDelegate Delegate;
	Delegate.BindUObject(this, &ThisClass::TweenDashCooldownUIParameter);
	UTweenSubsystem* TweenSubsystem = Character->GetGameInstance()->GetSubsystem<UTweenSubsystem>();
	TweenSubsystem->AddTween(Handle, 0, Cooldown, Delegate);

	if (GetOwningComponent()->ActiveGameplayTags.HasTagExact(OverchargeTag))
	{
		const FVector TraceStart = Character->GetActorLocation();
		const float Radius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
		const float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FCollisionShape Shape;
		Shape.SetCapsule(Radius, HalfHeight);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Character);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		TArray<FHitResult> HitResults;

		if (GetWorld()->SweepMultiByObjectType(HitResults, TraceStart, DashLocation, FQuat::Identity, ObjParams, Shape))
		{
			const UCCombatStatusComponent* CombatStatusComponent = UCCombatStatusComponent::GetCombatStatusComponent(Character);
			FAttackData AttackData;
			AttackData.Instigator = Character;
			AttackData.AttackStatusType = CombatStatusComponent->GetAttackStatusType();
			AttackData.ImpactStrength = 4000.f;

			TArray<AActor*> ActorsHit;
		
			for (const FHitResult& HitResult: HitResults)
			{
				if (UCGameplayFunctionLibrary::TryRegisterHit(AttackData, HitResult.GetActor()))
				{
					ActorsHit.AddUnique(HitResult.GetActor());
				}
			}

			if (ActorsHit.Num() > 0)
			{
				UCGameplayFunctionLibrary::AddStatusReportMessage(Character, FString::Printf(TEXT("Hyperspeed enemies sliced: %d"), ActorsHit.Num()));
			}
		}
	}
	
	Character->SetActorLocation(DashLocation, false, nullptr, ETeleportType::ResetPhysics);
	const FVector VelocityDirection = Character->GetActorForwardVector();
	MovementComponent->Velocity = VelocityDirection * VelocityAfterDash;

	if (MovementComponent->IsMovingOnGround())
	{
		StopAction(Character);
		bGroundTouched = true;
	}
	
	const FString Message = FString::Printf(TEXT("%s used."), *ActionName.ToString());
	UCGameplayFunctionLibrary::AddStatusReportMessage(GetOuter(), Message);
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

FString UCAction_Dash::GetInCooldownMessage() const
{
	return FString::Printf(TEXT("%s refueling"), *ActionName.ToString());
}

void UCAction_Dash::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMovementMode)
{
	if (PreviousMovementMode == MOVE_Falling)
	{
		if (bDashing)
			InterruptDash();
		bGroundTouched = true;
	}
}

void UCAction_Dash::TweenDashCooldownUIParameter(float Value)
{
	const float ActualValue = Value / Cooldown;
	UKismetMaterialLibrary::SetScalarParameterValue(Character, UIMaterialParameters, TEXT("DashCooldownProgress"), ActualValue);
}

void UCAction_Dash::GrantDashCharge()
{
	if (bDashing)
		InterruptDash();
	
	bGroundTouched = true;
}
