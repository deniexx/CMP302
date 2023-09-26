// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CCharacterMovementComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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

void UCCharacterMovementComponent::BeginDash()
{
	if (bDashConsumed || bDashing) return;
	if (DashTimerHandle.IsValid()) return;
	
	bDashing = true;
	AirControl = 1.f;
	MaxWalkSpeed = 2 * MaxWalkSpeed;
	MaxAcceleration = 2 * MaxAcceleration;
	UGameplayStatics::SetGlobalTimeDilation(GetOwner(), 0.2f);
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UCCharacterMovementComponent::InterruptDash, DashTimeSlowDuration);
}

void UCCharacterMovementComponent::Dash()
{
	FVector DashLocation;
	ACharacter* Owner = GetCharacterOwner();
	FVector TraceStart = Owner->GetPawnViewLocation();
	FVector TraceEnd = TraceStart + (Owner->GetControlRotation().Vector() * DashDistance);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	// @TODO: Look into dashing through enemy to score a kill, this can potentially reset dash too
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		DashLocation = HitResult.ImpactPoint;
	}
	else
	{
		DashLocation = TraceEnd;
	}
	
	FVector Offset = DashLocation - TraceStart;
	GetOwner()->AddActorWorldOffset(Offset, true, nullptr, ETeleportType::TeleportPhysics);
	SetMovementMode(MOVE_Falling);
}

void UCCharacterMovementComponent::ResetDash()
{
	if (bDashing || bDashConsumed)
	{
		AirControl = DefaultAirControl;
		MaxWalkSpeed = DefaultMaxWalkSpeed;
		MaxAcceleration = DefaultMaxAcceleration;
		bDashing = false;
		UGameplayStatics::SetGlobalTimeDilation(GetOwner(), 1.f);

		if (DashTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
			DashTimerHandle.Invalidate();
		}
	}
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
	ResetDash();
	RefillDash();
}

void UCCharacterMovementComponent::RefillDash()
{
	bDashConsumed = false;
	ResetDash();
}

void UCCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Falling)
	{
		RefillDash();
	}
}
