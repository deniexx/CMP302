// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CCharacterMovementComponent.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CDashUserWidget.h"

static TAutoConsoleVariable<int32> CVarShowDebugTraversal(
	TEXT("ShowDebugTraversal"),
	0,
	TEXT("Draws debug info about traversal")
	TEXT(" 0: Do not show debug info/n")
	TEXT(" 1: Show Debug info/n"),
	ECVF_Cheat
);

UCCharacterMovementComponent::UCCharacterMovementComponent()
{
	bDashing = false;
	bDashConsumed = false;
	DashTimeSlowDuration = 1.5f;
	DefaultAirControl = 0.05f;
	DashDistance = 750.f;
	DefaultMaxWalkSpeed = 600;
	DefaultMaxAcceleration = 2048;
	WallRunJumpOffVelocity = 420.f;
	VelocityAfterDash = 150.f;
	bWallRunning = false;
}

void UCCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	DefaultAirControl = AirControl;
	DefaultMaxWalkSpeed = MaxWalkSpeed;
	DefaultMaxAcceleration = GetMaxAcceleration();
	
	if (const APlayerController* Controller = GetCharacterOwner() ? GetCharacterOwner()->GetController<APlayerController>() : nullptr)
	{
		UCapsuleComponent* CapsuleComponent = GetCharacterOwner()->GetCapsuleComponent();
		CapsuleComponent->OnComponentHit.AddDynamic(this, &UCCharacterMovementComponent::OnCapsuleComponentHit);
	}
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
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		const float ScaledCapsuleHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		bool bHit = GetWorld()->LineTraceSingleByChannel(WallRunHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
		if (bHit)
		{
			TraceEnd = WallRunHit.ImpactPoint;
			/** Do one more line trace in case to check for the bottom location of the Indicator */
			FVector Down = TraceEnd;
			FCollisionShape Shape;
			Shape.SetCapsule(CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius(), CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			Down.Z -= 10000000.f;
			if (GetWorld()->SweepSingleByChannel(WallRunHit, TraceEnd, Down, FQuat::Identity, ECC_Visibility, Shape, QueryParams))
			{
				IndicatorLocation = WallRunHit.ImpactPoint;
				DashLocation = WallRunHit.ImpactPoint + FVector(0.f, 0.f, ScaledCapsuleHalfHeight);
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
	else if (bWallRunning)
	{
		FHitResult WallHit;
		if (!FindRunnableWall(WallHit))
		{
			EndWallRun();
			return;
		}
		
		const FRotator DirectionRotation = IsWallOnTheLeft(WallHit) ? FRotator(0, -90, 0) : FRotator(0, 90, 0);
		const FVector WallRunDirection = DirectionRotation.RotateVector(WallHit.ImpactNormal);
		
		/** Slighty faster than the max walk speed */
		Velocity = WallRunDirection * (MaxWalkSpeed + 100.f);
		Velocity.Z = 0;
		SetPlaneConstraintEnabled(true);
		SetPlaneConstraintOrigin(WallHit.ImpactPoint);
		SetPlaneConstraintNormal(WallHit.ImpactNormal);
	}
}

#pragma region Dash
void UCCharacterMovementComponent::BeginDash()
{
	if (bDashConsumed || bDashing || bWallRunning) return;
	if (DashTimerHandle.IsValid()) return;

	bDashConsumed = true;
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
	GetOwner()->SetActorLocation(DashLocation, false, nullptr, ETeleportType::ResetPhysics);
	const FVector VelocityDirection = GetCharacterOwner()->GetActorForwardVector();
	Velocity = VelocityDirection * VelocityAfterDash;

	if (IsMovingOnGround())
		bDashConsumed = false;
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
	if (bDashing)
	{
		Dash();
		ResetDash();
	}
}

void UCCharacterMovementComponent::InterruptDash()
{
	ResetDash();

	if (IsMovingOnGround())
		bDashConsumed = false;
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
#pragma endregion Dash

void UCCharacterMovementComponent::BeginTraversal()
{
	if (GetCharacterOwner()->CanJump() || (IsJumpAllowed() && bWallRunning))
	{
		if (bWallRunning)
		{
			EndWallRun();
			const FVector JumpOffDirection = GetCharacterOwner()->GetActorForwardVector();
			const FVector JumpOffVector = GetCurrentAcceleration().GetSafeNormal() + FVector::UpVector + JumpOffDirection;
			
			GetCharacterOwner()->LaunchCharacter(JumpOffVector * WallRunJumpOffVelocity, true, true);
		}
		else
		{
			GetCharacterOwner()->Jump();
		}
	}
}

void UCCharacterMovementComponent::OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsMovingOnGround() && !bWallRunning)
	{
		BeginWallRun();
	}
}

void UCCharacterMovementComponent::BeginWallRun()
{
	if (!FindRunnableWall(WallRunHit) || bDashing)
	{
		EndWallRun();
		
		return;
	}

	if (const APlayerController* PlayerController = GetCharacterOwner()->GetController<APlayerController>())
	{
		AppliedCameraModifier = PlayerController->PlayerCameraManager->AddNewCameraModifier(IsWallOnTheLeft(WallRunHit) ? LeftWallRunCameraModifier : RightWallRunCameraModifier);
	}
	
	GetCharacterOwner()->Landed(WallRunHit);
	GetCharacterOwner()->SetActorLocation(WallRunHit.ImpactPoint + WallRunHit.ImpactNormal * 60.f);
	SetMovementMode(MOVE_Flying);
	GravityScale = 0.2f;

	bWallRunning = true;
}

void UCCharacterMovementComponent::EndWallRun()
{
	SetPlaneConstraintEnabled(false);
	SetMovementMode(MOVE_Falling);

	if (const APlayerController* PlayerController = GetCharacterOwner()->GetController<APlayerController>())
	{
		PlayerController->PlayerCameraManager->RemoveCameraModifier(AppliedCameraModifier);
	}
	GravityScale = 1.f;
	bWallRunning = false;
}

bool UCCharacterMovementComponent::FindRunnableWall(FHitResult& OutWallHit) const
{
	const FVector CharacterLocation = GetCharacterOwner()->GetActorLocation();
	const FVector RightVector = GetCharacterOwner()->GetActorRightVector();
	const FVector ForwardVector = GetCharacterOwner()->GetActorForwardVector();

	const float TraceLength = GetCharacterOwner()->GetCapsuleComponent()->GetScaledCapsuleRadius() + 60.f;

	const TArray<AActor*> IgnoredActors = {GetCharacterOwner()};
	
	const bool bShowTraversal = CVarShowDebugTraversal.GetValueOnAnyThread() > 0;;

	const EDrawDebugTrace::Type DebugDrawType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	
	/** This hit means there is something in front of us so we can't wall run further or begin */
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + ForwardVector * TraceLength, WallRunObjectTypes, true, IgnoredActors, DebugDrawType, OutWallHit, true))
	{
		return false;
	}
	
	const float AcceptableAngle = bWallRunning ? 0.f : 0.3f;
	/** There is a wall to our left that we can wall run on */
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + (-RightVector * TraceLength), WallRunObjectTypes, true, IgnoredActors, DebugDrawType, OutWallHit, true))
	{
		/** Check if the angle for wall run is acceptable */
		
		if (FVector::DotProduct(OutWallHit.ImpactNormal, RightVector) > AcceptableAngle)
		{
			return true;
		}
	}

	/** There is a wall to our right that we can wall run on */
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + RightVector * TraceLength, WallRunObjectTypes, true, IgnoredActors, DebugDrawType, OutWallHit, true))
	{
		/** Check if the angle for wall run is acceptable */
		if (FVector::DotProduct(OutWallHit.ImpactNormal, -RightVector) > AcceptableAngle)
		{
			return true;
		}
	}

	/** There isn't a wall runnable wall */
	return false;
}

bool UCCharacterMovementComponent::IsWallOnTheLeft(const FHitResult& InWallHit) const
{
	return FVector::DotProduct(GetCharacterOwner()->GetActorRightVector(), InWallHit.ImpactNormal) > 0.f;
}
