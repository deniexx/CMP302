// Coded by: Denis Dimitrov for CMP302


#include "GameModes/CArenaGameMode.h"

#include "CLogChannels.h"
#include "EngineUtils.h"
#include "Character/CAICharacter.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<int32> CVarSpawnBots(
	TEXT("DisableSpawnBots"),
	true,
	TEXT("Disable spawning of bots via timer.")
	TEXT("1 - Off")
	TEXT("2 - On"),
	ECVF_Cheat);

void ACArenaGameMode::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotsHandle, this, &ThisClass::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ACArenaGameMode::SpawnBotTimerElapsed()
{
	if(CVarSpawnBots.GetValueOnGameThread() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CvasSpawnBots'."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnQueryFinished);
	}
}

void ACArenaGameMode::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogCMP, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	int32 NumAliveBots = 0;
	for (TActorIterator<ACAICharacter> It(GetWorld()); It; ++It)
	{
		ACAICharacter* Bot = *It;

		if (!Bot->IsDead())
		{
			++NumAliveBots;
		}
	}

	UE_LOG(LogCMP, Log, TEXT("Found %d alive bots."), NumAliveBots);

	float MaxBotCount = 10.f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumAliveBots >= MaxBotCount)
	{
		UE_LOG(LogCMP, Log, TEXT("At maximum bot capacity. Skipping spawn."))
		return;
	}

	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();

	if (SpawnLocations.Num() > 0)
	{
		AActor* NewBot = GetWorld()->SpawnActor<AActor>(BotClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}