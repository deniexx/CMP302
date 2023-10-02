// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_PlayCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class CMP302_API UCAnimNotify_PlayCameraShake : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShakeBase;
};
