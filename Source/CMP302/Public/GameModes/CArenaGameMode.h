// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CMP302GameMode.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "CArenaGameMode.generated.h"

struct FAttackData;
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

	ACArenaGameMode();
	
	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:

	/** The interval time between enemy spawns */
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	UCurveFloat* SpawnIntervalCurve;

	/** The bot class used as a template to spawn enemies */
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	TSubclassOf<ACAICharacter> BotClass;

	/** The environment query for bot spawns */
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	UEnvQuery* SpawnBotQuery;

	/** The difficulty curve for max number of enemies spawned */
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	UCurveFloat* DifficultyCurve;

private:
	
	UFUNCTION()
	void OnBotKilled(const FAttackData& AttackData);

	/** Number of bots killed */
	UPROPERTY(BlueprintReadOnly, Category = "Arena", meta = (AllowPrivateAccess = true))
	int32 BotsKilled;

	/** Time survived in seconds */
	UPROPERTY(BlueprintReadOnly, Category = "Arena", meta = (AllowPrivateAccess = true))
	float TimeSurvived;

	/** On spawn bot query finished, this event will spawn an enemy at the best position */
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void SpawnBotTimerElapsed();

	FTimerHandle SpawnBotsHandle;
};
