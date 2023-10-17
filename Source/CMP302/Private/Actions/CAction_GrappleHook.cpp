// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_GrappleHook.h"

#include "CLogChannels.h"
#include "UI/CWorldUserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/CPlayerCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

static TAutoConsoleVariable<int32> CVarShowDebugGrappleHook(
	TEXT("ShowDebugGrappleHook"),
	0,
	TEXT("Draws debug info about grapple hook")
	TEXT(" 0: Do not show debug info/n")
	TEXT(" 1: Show Debug info/n"),
	ECVF_Cheat
);

void UCAction_GrappleHook::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Character = Cast<ACPlayerCharacter>(InInstigator);
}

void UCAction_GrappleHook::TickAction_Implementation(float DeltaTime)
{
	Super::TickAction_Implementation(DeltaTime);

	const FVector TraceStart = Character->GetActorLocation();
	const TArray<AActor*> IgnoredActors = { Character };

	const bool bShowDebug = CVarShowDebugGrappleHook.GetValueOnAnyThread() > 0;
	const EDrawDebugTrace::Type DebugDrawType = bShowDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	TArray<FHitResult> HitResults;
	
	if (UKismetSystemLibrary::SphereTraceMultiForObjects(Character, TraceStart, TraceStart, MaxGrappleRange, GrappleHookObjectTypes, false, IgnoredActors, DebugDrawType, HitResults, true))
	{
		FindBestTarget(HitResults);
		if (GrappleTarget)
		{
			if (!WorldUserWidgetInstance)
			{
				APlayerController* PlayerController = Character->GetController<APlayerController>(); 
				WorldUserWidgetInstance = CreateWidget<UCWorldUserWidget>(PlayerController, WorldUserWidgetClass);
			}
			
			WorldUserWidgetInstance->AttachedActor = GrappleTarget;
			if (!WorldUserWidgetInstance->IsInViewport())
				WorldUserWidgetInstance->AddToViewport();
		}
		else
		{
			GrappleTarget = nullptr;
			if (WorldUserWidgetInstance && WorldUserWidgetInstance->IsInViewport())
				WorldUserWidgetInstance->RemoveFromParent();
		}
	}
	else
	{
		GrappleTarget = nullptr;
		if (WorldUserWidgetInstance && WorldUserWidgetInstance->IsInViewport())
			WorldUserWidgetInstance->RemoveFromParent();
	}
}

void UCAction_GrappleHook::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	Character->PlayAnimMontage(GrappleMontage);
	
	FVector Direction = GrappleTarget->GetActorLocation() - Character->GetActorLocation();
	const float Distance = Direction.Length();
	Direction.Normalize();

	if (!NiagaraComponent)
	{
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(GrappleEffect, Character->GetMesh(), TEXT("hand_l"), FVector(0.f), FRotator(0.f), EAttachLocation::SnapToTarget, false);
	}
	else
	{
		NiagaraComponent->Activate(true);
	}

	NiagaraComponent->SetNiagaraVariableVec3(FString("StartBeam"), Character->GetMesh()->GetBoneLocation(TEXT("hand_l")));
	NiagaraComponent->SetNiagaraVariableVec3(FString("EndBeam"), GrappleTarget->GetActorLocation());

	Character->LaunchCharacter(Direction * Distance * GrapplePower, true, true);
}

void UCAction_GrappleHook::StopAction_Implementation(AActor* InInstigator)
{
	Super::StopAction_Implementation(InInstigator);

	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}
}

bool UCAction_GrappleHook::CanStart_Implementation(AActor* InInstigator)
{
	return Super::CanStart_Implementation(InInstigator) && GrappleTarget;
}

void UCAction_GrappleHook::FindBestTarget(const TArray<FHitResult>& HitResults)
{
	FHitResult Hit;
	GrappleTarget = nullptr;

	FCollisionShape Shape;
	Shape.SetSphere(50.f);
	
	float BestAngle = 360.f;
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* Target = HitResult.GetActor();
		const FVector TraceStart = Character->GetFirstPersonCameraComponent()->GetComponentLocation();
		const FVector TraceEnd = Target->GetActorLocation();

		// If we don't hit we don't see the object
		if (GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, Shape))
		{
			if (HitResult.GetActor() != Hit.GetActor())
			{
				// We hit something else, meaning we don't have line of sight
				continue;
			}
			FVector ToTarget = (TraceEnd - Character->GetActorLocation());
			ToTarget.Normalize();
			const FVector CameraForward = Character->GetFirstPersonCameraComponent()->GetForwardVector();

			const float AngleDeg = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ToTarget, CameraForward)));

			if (AngleDeg > 30.f)
				continue; // Angle is too much, so we don't accept it as a valid target
			
			if (AngleDeg < BestAngle || !GrappleTarget)
			{
				BestAngle = AngleDeg;
				GrappleTarget = Target;
			}
		}
	}
}
