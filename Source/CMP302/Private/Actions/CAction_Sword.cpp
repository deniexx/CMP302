// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_Sword.h"

#include "ActorComponents/CCombatStatusComponent.h"
#include "Character/CCommonCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Projectiles/CProjectile.h"
#include "System/CGameplayFunctionLibrary.h"

static TAutoConsoleVariable<int32> CVarShowDebugSwordAttack(
	TEXT("ShowDebugSwordAttack"),
	0,
	TEXT("Draws debug info about sword attacks")
	TEXT(" 0: Do not show debug info/n")
	TEXT(" 1: Show Debug info/n"),
	ECVF_Cheat
);

void UCAction_Sword::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	ACCommonCharacter* Character = Cast<ACCommonCharacter>(InInstigator);

	NextComboAttack = 1;
	Character->GetMesh()->SetHiddenInGame(false);
	Character->ReadyActor();
	SwordMeshComponent = Cast<UStaticMeshComponent>(InInstigator->AddComponentByClass(UStaticMeshComponent::StaticClass(), true, FTransform(), false));
	SwordMeshComponent->SetStaticMesh(SwordMesh);
	SwordMeshComponent->SetMobility(EComponentMobility::Movable);
	SwordMeshComponent->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r"));
	SwordMeshComponent->SetRelativeTransform(AttachTransform);
	SwordMeshComponent->bCastDynamicShadow = false;
	SwordMeshComponent->CastShadow = false;
	SwordMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UCAction_Sword::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);
	
	SwordMeshComponent->DestroyComponent();
}

void UCAction_Sword::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	ACCommonCharacter* Character = Cast<ACCommonCharacter>(InInstigator);

	if (IsTerrainInFront(Character))
	{
		Character->PlayAnimMontage(ReflectAnimMontage, 2.f);
		return;
	}

	if (ReflectProjectileIfPossible(Character))
	{
		Character->PlayAnimMontage(ReflectAnimMontage, 1.5f);
		return;
	}
	
	SnapToTargetIfPossible(Character);
	TraceForEnemyHits(Character);

	if (NextComboAttack == 1)
	{
		const uint32 Index = FMath::RandRange(0, SlashAttackMontages.Num() - 1);
		CurrentMontage = SlashAttackMontages[Index];
	}

	const FString ComboMontage = FString::Printf(TEXT("Attack%d"), NextComboAttack);
	Character->PlayAnimMontage(CurrentMontage, 1, FName(*ComboMontage));

	if (ResetComboTimer_Handle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ResetComboTimer_Handle);
	}

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &ThisClass::ResetComboTimerElapsed, Character);
	GetWorld()->GetTimerManager().SetTimer(ResetComboTimer_Handle, Delegate, 0.75f, false);
	
	if (++NextComboAttack > 3)
		NextComboAttack = 1;
}

void UCAction_Sword::ResetComboTimerElapsed(ACCommonCharacter* Character)
{
	ResetComboTimer_Handle.Invalidate();
	NextComboAttack = 1;
	Character->StopAnimMontage(CurrentMontage);
}

bool UCAction_Sword::IsTerrainInFront(ACCommonCharacter* Character) const
{
	FVector TraceStart = Character->GetPawnViewLocation();
	
	// Using max reflect distance here, as it's range is quite short
	FVector TraceEnd = TraceStart + (Character->GetControlRotation().Vector() * MaxReflectDistance);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	
	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjParams, QueryParams);

	return HitResult.bBlockingHit;
}

bool UCAction_Sword::SnapToTargetIfPossible(ACCommonCharacter* Character) const
{
	FVector TraceStart = Character->GetPawnViewLocation();
	FVector TraceEnd = TraceStart + (Character->GetControlRotation().Vector() * MaxSnapDistance);
	FCollisionQueryParams QueryParams;

	FCollisionShape Shape;
	Shape.SetSphere(20.f);
	QueryParams.AddIgnoredActor(Character);

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	TArray<FHitResult> HitResults;
		
	if (GetWorld()->SweepMultiByObjectType(HitResults, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, QueryParams))
	{
		for (const FHitResult& HitResult : HitResults)
		{
			FVector DirectionVector = HitResult.ImpactPoint - TraceStart;
			if (DirectionVector.Length() > MinSnapDistance)
			{
				FVector EndLocation = HitResult.ImpactPoint - (DirectionVector.GetSafeNormal() * 30.f);
				FVector ActorLocation = Character->GetActorLocation();
				if (EndLocation.Z > ActorLocation.Z)
					EndLocation.Z = ActorLocation.Z;
				Character->SetActorLocation(EndLocation, true);

				UCGameplayFunctionLibrary::AddStatusReportMessage(Character, TEXT("Boosting to enemy to slice"));
				return true;
			}
		}
	}

	return false;
}

bool UCAction_Sword::ReflectProjectileIfPossible(ACCommonCharacter* Character) const
{
	const FVector TraceStart = Character->GetActorLocation();
	const FVector TraceEnd = TraceStart + (Character->GetControlRotation().Vector() * MaxReflectDistance);
	FCollisionQueryParams QueryParams;

	FCollisionShape Shape;
	Shape.SetSphere(ReflectSphereSize);
	QueryParams.AddIgnoredActor(Character);

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_GameTraceChannel1); // Projectile channel

	TArray<FHitResult> HitResults;
	bool bSuccess = false;
	
	if (GetWorld()->SweepMultiByObjectType(HitResults, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, QueryParams))
	{
		for (const FHitResult& HitResult : HitResults)
		{
			ACProjectile* Projectile = Cast<ACProjectile>(HitResult.GetActor());
			if (!Projectile) continue;

			const FVector EndPosition = Projectile->GetOwner()->GetActorLocation();
			ACProjectile* SpawnedProjectile = UCGameplayFunctionLibrary::SpawnProjectileWithStarAndEndPosition(Character, Projectile->StaticClass(), Character, HitResult.ImpactPoint, EndPosition);
			SpawnedProjectile->SetAttackStatus(EAttackStatusType::White);
			UCGameplayFunctionLibrary::AddStatusReportMessage(Character, TEXT("Successfully reflected attack"));
			
			Projectile->Destroy();
			bSuccess |= true;
		}
	}

	return bSuccess;
}



bool UCAction_Sword::TraceForEnemyHits(ACCommonCharacter* Character) const
{
	const UCCombatStatusComponent* CombatStatusComponent = UCCombatStatusComponent::GetCombatStatusComponent(Character);
	if (!CombatStatusComponent) return false;
	
	FVector TraceStart = Character->GetActorLocation();

	TArray<AActor*> HitActors;
	const TArray<AActor*> IgnoredActors = { Character };
	TArray<FHitResult> HitResults;

	const bool bShowDebug = CVarShowDebugSwordAttack.GetValueOnAnyThread() > 0;
	const EDrawDebugTrace::Type DebugDrawType = bShowDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	for (int i = AttackTraceSphereMaxAmount; i > 0; --i)
	{
		TraceStart += Character->GetActorForwardVector() * AttackTraceLength;

		const FVector ActualTraceStart = TraceStart + Character->GetActorRightVector() * (i * AttackTraceWidth);
		const FVector ActualTraceEnd = TraceStart - Character->GetActorRightVector() * (i * AttackTraceWidth);

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), ActualTraceStart, ActualTraceEnd, AttackTraceSphereSize, AttackObjectTypes, true, IgnoredActors, DebugDrawType, HitResults, true);
		
		for (const FHitResult& Result : HitResults)
		{
			if (AActor* HitActor = Result.GetActor())
			{
				HitActors.Add(HitActor);
			}
		}
	}

	bool bSuccess = false;
	
	for (AActor* HitActor : HitActors)
	{
		FAttackData AttackData;
		AttackData.Instigator = InstigatorActor;
		AttackData.AttackStatusType = CombatStatusComponent->GetAttackStatusType();
		AttackData.ImpactStrength = 3000.f;
		bSuccess |= UCGameplayFunctionLibrary::TryRegisterHit(AttackData, HitActor);
	}

	return bSuccess;
}
