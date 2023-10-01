// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_Slide.h"

#include "ActorComponents/CActionComponent.h"
#include "ActorComponents/CExtendedCharacterMovement.h"
#include "Character/CCommonCharacter.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CapsuleComponent.h"

void UCAction_Slide::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Character = Cast<ACPlayerCharacter>(InInstigator);
	MovementComponent = Character->GetCharacterMovement<UCExtendedCharacterMovement>();
	
	StandingCapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	bCrouching = false;
	bSliding = false;
	CrouchAlpha = 0.f;
}

void UCAction_Slide::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);

	
}

void UCAction_Slide::TickAction_Implementation(float DeltaTime)
{
	Super::TickAction_Implementation(DeltaTime);

	const float TargetAlpha = bCrouching ? 1.f : 0.f;
	const bool bCanProceed = bCrouching ? true : CanUncrouch();
	
	if (!FMath::IsNearlyEqual(CrouchAlpha, TargetAlpha) && bCanProceed)
	{
		CrouchAlpha += bCrouching ? DeltaTime * 5 : -DeltaTime * 5;
		CrouchAlpha = FMath::Clamp(CrouchAlpha, 0.f, 1.f);
		const float HalfHeight = FMath::Lerp(StandingCapsuleHalfHeight, MovementComponent->GetCrouchedHalfHeight(), CrouchAlpha);
		Character->GetCapsuleComponent()->SetCapsuleHalfHeight(HalfHeight);
	}
	else if (FMath::IsNearlyZero(CrouchAlpha))
	{
		GetOwningComponent()->ActiveGameplayTags.RemoveTags(CrouchingTags);
	}
	else if (FMath::IsNearlyEqual(CrouchAlpha, 1.f))
	{
		GetOwningComponent()->ActiveGameplayTags.AppendTags(CrouchingTags);
	}
	
	if (bSliding && IsOnGround())
	{
		const FHitResult& Floor = MovementComponent->CurrentFloor.HitResult;
		
		if (IsFloorDownwardSlope(Floor.Normal))
		{
			MovementComponent->AddImpulse(Character->GetActorForwardVector() * SlideForce);
		}
		else if (CurrentSlideDuration < OnGroundSlideDuration)
		{
			CurrentSlideDuration += DeltaTime;
			MovementComponent->AddImpulse(Character->GetActorForwardVector() * SlideForce);
		}
		else
		{
			EndSliding();
		}
		
		const float HorizontalVelocity = FMath::Abs(MovementComponent->Velocity.X) + FMath::Abs(MovementComponent->Velocity.Y);
		if (HorizontalVelocity > MaxSlideSpeed)
		{
			const FVector XYVelocity = MovementComponent->Velocity.GetSafeNormal() * MaxSlideSpeed;
			MovementComponent->Velocity.X = XYVelocity.X;
			MovementComponent->Velocity.Y = XYVelocity.Y;
		}
	}
}

void UCAction_Slide::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);
	
	bCrouching = true;

	DefaultMaxWalkSpeed = MovementComponent->MaxWalkSpeed;
	MovementComponent->MaxWalkSpeed = MovementComponent->MaxWalkSpeedCrouched;
	const float HorizontalVelocity = FMath::Abs(MovementComponent->Velocity.X) + FMath::Abs(MovementComponent->Velocity.Y);
	if (HorizontalVelocity > 100.f)
	{
		BeginSliding();
	}
}

void UCAction_Slide::StopAction_Implementation(AActor* InInstigator)
{
	Super::StopAction_Implementation(InInstigator);

	MovementComponent->MaxWalkSpeed = DefaultMaxWalkSpeed;
	bCrouching = false;
	EndSliding();
}

bool UCAction_Slide::CanStart_Implementation(AActor* InInstigator)
{
	const bool bCanCrouch = MovementComponent->CanEverCrouch() && MovementComponent->CanCrouchInCurrentState();
	return Super::CanStart_Implementation(InInstigator) && bCanCrouch;
}

void UCAction_Slide::BeginSliding()
{
	bSliding = true;
}

bool UCAction_Slide::IsFloorDownwardSlope(const FVector& FloorNormal)
{
	const FVector Across =  FVector::CrossProduct(FVector::UpVector, FloorNormal);
	const FVector DownSlope = FVector::CrossProduct(Across, FloorNormal);

	return DownSlope.GetSafeNormal().Length() > 0.f;
}

bool UCAction_Slide::CanUncrouch() const
{
	if (bCrouching) return false;

	FVector TraceStart = Character->GetActorLocation();
	const FVector FeetLocation = TraceStart - FVector(0.f, 0.f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	TraceStart = FeetLocation + FVector(0.f, 0.f, StandingCapsuleHalfHeight * 2);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	GetWorld()->LineTraceSingleByChannel(HitResult, FeetLocation, TraceStart, ECC_Visibility, Params);

	return !GetWorld()->LineTraceSingleByChannel(HitResult, FeetLocation, TraceStart, ECC_Visibility, Params);;
}

bool UCAction_Slide::IsOnGround() const
{
	return !GetOwningComponent()->ActiveGameplayTags.HasTagExact(InAirTag);
}

void UCAction_Slide::EndSliding()
{
	CurrentSlideDuration = 0.f;
	bSliding = false;
}
