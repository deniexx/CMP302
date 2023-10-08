// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_WallRun.h"

#include "ActorComponents/CActionComponent.h"
#include "Camera/CameraModifier.h"
#include "Character/CCommonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/CGameplayFunctionLibrary.h"

static TAutoConsoleVariable<int32> CVarShowDebugWallRun(
	TEXT("ShowDebugWallRun"),
	0,
	TEXT("Draws debug info about wall run")
	TEXT(" 0: Do not show debug info/n")
	TEXT(" 1: Show Debug info/n"),
	ECVF_Cheat
);

void UCAction_WallRun::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Character = Cast<ACCommonCharacter>(InInstigator);
	MovementComponent = Character->GetCharacterMovement();
	Character->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UCAction_WallRun::OnCapsuleComponentHit);
	Character->OnCharacterLanded.AddDynamic(this, &UCAction_WallRun::OnCharacterLanded);

	if (const APlayerController* PlayerController = Character->GetController<APlayerController>())
    {
    	AppliedCameraModifierLeft = PlayerController->PlayerCameraManager->AddNewCameraModifier(LeftWallRunCameraModifier);  
		AppliedCameraModifierRight = PlayerController->PlayerCameraManager->AddNewCameraModifier(RightWallRunCameraModifier);
		AppliedCameraModifierLeft->DisableModifier();
		AppliedCameraModifierRight->DisableModifier();
    }
}

void UCAction_WallRun::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);

	Character->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &UCAction_WallRun::OnCapsuleComponentHit);

	if (const APlayerController* PlayerController = Character->GetController<APlayerController>())
	{
		PlayerController->PlayerCameraManager->RemoveCameraModifier(AppliedCameraModifierLeft);
		PlayerController->PlayerCameraManager->RemoveCameraModifier(AppliedCameraModifierRight);
	}
}

void UCAction_WallRun::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	GetOwningComponent()->ActiveGameplayTags.AppendTags(InAirTags);
	
	if (bWallRunning)
	{
		EndWallRun();
		const FVector JumpOffDirection = Character->GetActorForwardVector();
		const FVector JumpOffVector = MovementComponent->GetCurrentAcceleration().GetSafeNormal() + FVector::UpVector + JumpOffDirection;
		
		Character->LaunchCharacter(JumpOffVector * WallRunJumpOffVelocity, true, true);
	}
	else
	{
		Character->Jump();
	}
}

void UCAction_WallRun::StopAction_Implementation(AActor* InInstigator)
{
	Super::StopAction_Implementation(InInstigator);
	
	Character->StopJumping();
}

bool UCAction_WallRun::CanStart_Implementation(AActor* InInstigator)
{
	if (!Super::CanStart_Implementation(InInstigator))
		return false;
	
	return Character->CanJump() || (MovementComponent->IsJumpAllowed() && bWallRunning);
}

void UCAction_WallRun::TickAction_Implementation(float DeltaTime)
{
	Super::TickAction_Implementation(DeltaTime);

	if (bWallRunning)
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
		MovementComponent->Velocity = WallRunDirection * (MovementComponent->MaxWalkSpeed + 100.f);
		MovementComponent->Velocity.Z = 0;
		MovementComponent->SetPlaneConstraintEnabled(true);
		MovementComponent->SetPlaneConstraintOrigin(WallHit.ImpactPoint);
		MovementComponent->SetPlaneConstraintNormal(WallHit.ImpactNormal);
	}
}

FString UCAction_WallRun::GetInCooldownMessage() const
{
	return FString::Printf(TEXT("%ls is refueling"), *ActionName.ToString());
}

void UCAction_WallRun::BeginWallRun()
{
	if (!FindRunnableWall(WallRunHit))
	{
		EndWallRun();
		
		return;
	}
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(WallRunningTags);
	Character->Landed(WallRunHit);
	Character->SetActorLocation(WallRunHit.ImpactPoint + WallRunHit.ImpactNormal * 60.f);
	MovementComponent->SetMovementMode(MOVE_Flying);
	MovementComponent->GravityScale = 0.2f;

	bWallRunning = true;

	const FString Message = FString::Printf(TEXT("%s engaged."), *ActionName.ToString());
	UCGameplayFunctionLibrary::AddStatusReportMessage(GetOuter(), Message);

	IsWallOnTheLeft(WallRunHit) ? AppliedCameraModifierLeft->EnableModifier() : AppliedCameraModifierRight->EnableModifier();
}

void UCAction_WallRun::EndWallRun()
{
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(WallRunningTags);
	MovementComponent = Character->GetCharacterMovement();
	MovementComponent->SetPlaneConstraintEnabled(false);
	MovementComponent->SetMovementMode(MOVE_Falling);
	
	MovementComponent->GravityScale = 1.f;
	bWallRunning = false;

	const FString Message = FString::Printf(TEXT("%s disengaged."), *ActionName.ToString());
	UCGameplayFunctionLibrary::AddStatusReportMessage(GetOuter(), Message);

	if (bWasLastWallRunOnLeft)
		AppliedCameraModifierLeft->DisableModifier();
	else
		AppliedCameraModifierRight->DisableModifier();
}

bool UCAction_WallRun::FindRunnableWall(FHitResult& OutWallHit) const
{
	const FVector CharacterLocation = Character->GetActorLocation();
	const FVector RightVector = Character->GetActorRightVector();
	const FVector ForwardVector = Character->GetActorForwardVector();

	const float TraceLength = Character->GetCapsuleComponent()->GetScaledCapsuleRadius() + 70.f;

	const TArray<AActor*> IgnoredActors = {Character};
	
	const bool bShowTraversal = CVarShowDebugWallRun.GetValueOnAnyThread() > 0;;

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

bool UCAction_WallRun::IsWallOnTheLeft(const FHitResult& InWallHit)
{
	bWasLastWallRunOnLeft = FVector::DotProduct(Character->GetActorRightVector(), InWallHit.ImpactNormal) > 0.f;
	return bWasLastWallRunOnLeft;
}

void UCAction_WallRun::OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!Character->GetCharacterMovement()->IsMovingOnGround() && !bWallRunning)
	{
		BeginWallRun();
	}
}

void UCAction_WallRun::OnCharacterLanded(const FHitResult& Hit)
{
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(InAirTags);
}
