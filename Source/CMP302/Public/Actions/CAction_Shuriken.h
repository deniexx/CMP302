// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CAction_SpecialAbility.h"
#include "CAction_Shuriken.generated.h"

class ACCommonCharacter;
class ACProjectile;
/**
 * 
 */
UCLASS()
class CMP302_API UCAction_Shuriken : public UCAction_SpecialAbility
{
	GENERATED_BODY()

public:
	
	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;

	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

	virtual void StartAction_Implementation(AActor* InInstigator) override;

protected:

	virtual FString GetInCooldownMessage() const override;

	/** The projectile class for the shuriken (this is spawned) */
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ACProjectile> ShurikenProjectileClass;

	/** The delay from playing the animation to throwing the shuriken */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	/** The shuriken mesh to be assigned */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UStaticMesh* ShurikenMesh;

	/** The transform to set the shuriken mesh to */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FTransform AttachTransform;

	/** The montage to play when the action is started */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* ShurikenThrowMontage;
	
	UPROPERTY()
	UStaticMeshComponent* ShurikenMeshComponent;

	UFUNCTION()
	void AttackDelayTimer_Elapsed(ACCommonCharacter* Character);
};
