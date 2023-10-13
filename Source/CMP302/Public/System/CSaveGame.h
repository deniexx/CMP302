// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CSaveGame.generated.h"

class UCAction;

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FString ActorName;

	UPROPERTY()
	bool bCleared = false;
};

USTRUCT()
struct FPlayerSpawnData
{
	GENERATED_BODY()

	UPROPERTY()
	FName LevelName;
	
	UPROPERTY()
	FTransform Transform;
};

/**
 * 
 */
UCLASS()
class CMP302_API UCSaveGame : public USaveGame
{
	GENERATED_BODY()

	
public:

	UPROPERTY()
	TArray<TSubclassOf<UCAction>> PlayerActions;

	UPROPERTY()
	TArray<FActorSaveData> SaveData;
	
	UPROPERTY()
	TArray<FPlayerSpawnData> SpawnData;

	UPROPERTY()
	int32 Currency = 0;
};
