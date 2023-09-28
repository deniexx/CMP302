// Coded by: Denis Dimitrov for CMP302


#include "AnimNotifies/CAnimNotify_FireProjectile.h"

#include "ActorComponents/CCombatComponent.h"

void UCAnimNotify_FireProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (UCCombatComponent* CombatComponent = UCCombatComponent::GetCombatComponent(MeshComp->GetOwner()))
		CombatComponent->SpawnProjectile();
}
