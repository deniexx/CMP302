// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CCharacterMovementComponent.h"

#include "Blueprint/UserWidget.h"
#include "Engine/StaticMeshActor.h"
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

void UCCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDashing && DashIndicatorActor)
	{
		ACharacter* Owner = GetCharacterOwner();
		FVector TraceStart = Owner->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (Owner->GetControlRotation().Vector() * DashDistance);
		FVector DashLocation = TraceEnd;
		FVector IndicatorLocation = TraceEnd;
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
		if (bHit)
		{
			DashLocation = HitResult.ImpactPoint;
			/** Do one more line trace in case to check for the bottom location of the Indicator */
			FVector Down = DashLocation;
			Down.Z -= 10000000.f;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, DashLocation, Down, ECC_Vehicle, QueryParams))
			{
				IndicatorLocation = HitResult.ImpactPoint;
			}
			else IndicatorLocation.Z -= 96.f; 
		}
		
		DashOffset = DashLocation - TraceStart;
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
		DashVisualOverlayInstance = CreateWidget(CharacterOwner->GetController<APlayerController>(), DashVisualOverlayInstanceClass);
	}

	DashVisualOverlayInstance->AddToViewport();
	DashIndicatorActor->SetActorHiddenInGame(false);
	
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UCCharacterMovementComponent::InterruptDash, DashTimeSlowDuration);
}

void UCCharacterMovementComponent::Dash()
{
	GetOwner()->AddActorWorldOffset(DashOffset, true, nullptr, ETeleportType::TeleportPhysics);
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
		DashIndicatorActor->SetActorHiddenInGame(true);

		if (DashVisualOverlayInstance)
			DashVisualOverlayInstance->RemoveFromParent();
		
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
