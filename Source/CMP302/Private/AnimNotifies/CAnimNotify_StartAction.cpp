// Coded by: Denis Dimitrov for CMP302


#include "AnimNotifies/CAnimNotify_StartAction.h"

#include "ActorComponents/CActionComponent.h"

void UCAnimNotify_StartAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (UCActionComponent* ActionComponent = UCActionComponent::GetActionComponent(MeshComp->GetOwner()))
	{
		ActionComponent->StartActionByTag(MeshComp->GetOwner(), ActionTag);
	}
}
