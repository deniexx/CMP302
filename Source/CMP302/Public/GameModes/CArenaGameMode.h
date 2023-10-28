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
	void OnBotKilled(const FAttackData& AttackData);

	UPROPERTY(BlueprintReadOnly, Category = "Arena", meta = (AllowPrivateAccess = true))
	int32 BotsKilled;

	UPROPERTY(BlueprintReadOnly, Category = "Arena", meta = (AllowPrivateAccess = true))
	float TimeSurvived;
	
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void SpawnBotTimerElapsed();

	FTimerHandle SpawnBotsHandle;
};
