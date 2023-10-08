// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_Sword.h"

#include "ActorComponents/CCombatStatusComponent.h"
#include "Character/CCommonCharacter.h"
#include "Projectiles/CProjectile.h"
#include "System/CGameplayFunctionLibrary.h"

void UCAction_Sword::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	ACCommonCharacter* Character = Cast<ACCommonCharacter>(InInstigator);
	
	Character->GetMesh()->SetHiddenInGame(false);
	Character->ReadyActor();
	SwordMeshComponent = Cast<UStaticMeshComponent>(InInstigator->AddComponentByClass(UStaticMeshComponent::StaticClass(), true, FTransform(), false));
	SwordMeshComponent->SetStaticMesh(SwordMesh);
	SwordMeshComponent->SetMobility(EComponentMobility::Movable);
	SwordMeshComponent->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r"));
	SwordMeshComponent->SetRelativeTransform(AttachTransform);
	SwordMeshComponent->bCastDynamicShadow = false;
	SwordMeshComponent->CastShadow = false;
	SwordMeshComponent->SetCollisionProfileName("OverlapAll");
	SwordMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordMeshComponent->IgnoreActorWhenMoving(Character, true);
	
	SwordMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &UCAction_Sword::OnSwordHit);
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

	const int index = FMath::RandRange(0, SlashAttackMontages.Num() - 1);

	if (UAnimMontage* AnimMontage = SlashAttackMontages[index])
	{
		SwordMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Character->PlayAnimMontage(AnimMontage);
		
		SnapToTargetIfPossible(Character);
		ReflectProjectileIfPossible(Character);
	}
}

void UCAction_Sword::StopAction_Implementation(AActor* InInstigator)
{
	Super::StopAction_Implementation(InInstigator);
	
	SwordMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UCAction_Sword::OnSwordHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const UCCombatStatusComponent* CombatStatusComponent = UCCombatStatusComponent::GetCombatStatusComponent(InstigatorActor);
	if (!CombatStatusComponent) return;
	
	FAttackData AttackData;
	AttackData.Instigator = InstigatorActor;
	AttackData.AttackStatusType = CombatStatusComponent->GetAttackStatusType();
	AttackData.ImpactStrength = 30000.f;
	UCGameplayFunctionLibrary::TryRegisterHit(AttackData, OtherActor);
}

void UCAction_Sword::SnapToTargetIfPossible(ACCommonCharacter* Character) const
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
				break;
			}
		}
	}
}

void UCAction_Sword::ReflectProjectileIfPossible(ACCommonCharacter* Character) const
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
		}
	}
}
