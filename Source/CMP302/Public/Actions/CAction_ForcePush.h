// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CAction_SpecialAbility.h"
#include "CAction_ForcePush.generated.h"

/**
 * 
 */
UCLASS()
class CMP302_API UCAction_ForcePush : public UCAction_SpecialAbility
{
	GENERATED_BODY()

public:

	virtual void StartAction_Implementation(AActor* InInstigator) override;

protected:

	/** The range of the push (radius of the collision sphere) */
	UPROPERTY(EditDefaultsOnly, Category = "Push")
	float PushRange;

	/** The force of the push */
	UPROPERTY(EditDefaultsOnly, Category = "Push")
	float PushForce;

	/** The forward offset for the push collision detection */
	UPROPERTY(EditDefaultsOnly, Category = "Push")
	float ForwardOffset;

	/** Objects types to query for the push */
	UPROPERTY(EditDefaultsOnly, Category = "Push")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	/** The anim montage played when we activate the action */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* ForcePushMontage;
};
