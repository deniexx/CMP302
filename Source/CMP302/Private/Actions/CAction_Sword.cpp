// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_Sword.h"

#include "ActorComponents/CCombatStatusComponent.h"
#include "Character/CCommonCharacter.h"
#include "System/CGameplayFunctionLibrary.h"

void UCAction_Sword::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	ACCommonCharacter* Character = Cast<ACCommonCharacter>(InInstigator);
	
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
