// Coded by: Denis Dimitrov for CMP302


#include "AnimNotifies/CAnimNotify_PlayCameraShake.h"

#include "Character/CPlayerCharacter.h"

void UCAnimNotify_PlayCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const ACPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ACPlayerCharacter>();
	const APlayerController* Controller = PlayerCharacter ? PlayerCharacter->GetController<APlayerController>() : nullptr;
	if (APlayerCameraManager* CameraManager = Controller ? Controller->PlayerCameraManager : nullptr)
	{
		CameraManager->StartCameraShake(CameraShakeBase);
	}
}
