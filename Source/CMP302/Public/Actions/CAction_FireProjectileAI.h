// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_FireProjectileAI.generated.h"

class ACProjectile;

/**
 * 
 */
UCLASS()
class CMP302_API UCAction_FireProjectileAI : public UCAction
{
	GENERATED_BODY()

public:

	virtual void StartAction_Implementation(AActor* InInstigator) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ACProjectile> ProjectileClass;
};
