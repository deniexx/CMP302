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

	UPROPERTY(EditDefaultsOnly, Category = "Push")
	float PushRange;

	UPROPERTY(EditDefaultsOnly, Category = "Push")
	float PushForce;

	UPROPERTY(EditDefaultsOnly, Category = "Push")
	float ForwardOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Push")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* ForcePushMontage;
};
