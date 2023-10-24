// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CMP302GameMode.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "CArenaGameMode.generated.h"

class UEnvQuery;
class ACAICharacter;
/**
 * 
 */
UCLASS()
class CMP302_API ACArenaGameMode : public ACMP302GameMode
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	TSubclassOf<ACAICharacter> BotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	UCurveFloat* DifficultyCurve;

private:
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void SpawnBotTimerElapsed();

	FTimerHandle SpawnBotsHandle;
};
