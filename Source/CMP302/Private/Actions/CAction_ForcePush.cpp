// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_ForcePush.h"
#include "Character/CCommonCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/CCombatStatusComponent.h"

static TAutoConsoleVariable<int32> CVarShowDebugForcePush(
	TEXT("ShowDebugForcePush"),
	0,
	TEXT("Draws debug info about force push")
	TEXT(" 0: Do not show debug info/n")
	TEXT(" 1: Show Debug info/n"),
	ECVF_Cheat
);

void UCAction_ForcePush::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	ACCommonCharacter* Character = Cast<ACCommonCharacter>(InInstigator);

	const FVector TraceStart = Character->GetActorLocation() + (Character->GetControlRotation().Vector() * ForwardOffset);
	const FVector TraceEnd = TraceStart;
	FCollisionQueryParams QueryParams;

	FCollisionShape Shape;
	Shape.SetSphere(PushRange);
	QueryParams.AddIgnoredActor(Character);

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	TArray<FHitResult> HitResults;
	bool bSuccess = false;

	TArray<AActor*> IgnoredActors = { Character };	

	const bool bShowDebug = CVarShowDebugForcePush.GetValueOnAnyThread() > 0;
	const EDrawDebugTrace::Type DebugDrawType = bShowDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), TraceStart, TraceEnd, PushRange, ObjectTypes, true, IgnoredActors, DebugDrawType, HitResults, true))
	{
		for (const FHitResult& Hit : HitResults)
		{	
			if (ACCommonCharacter* HitCharacter = Cast<ACCommonCharacter>(Hit.GetActor()))
			{
				FVector Direction = HitCharacter->GetActorLocation() - Character->GetActorLocation();
				float Power = 1 - (Direction.Length() / (PushRange + ForwardOffset));
				Direction.Normalize();
				HitCharacter->GetCharacterMovement()->AddImpulse(Direction * Power * PushForce, true);
			}
		}
	}

	StopAction(InInstigator);
}
