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

/**
 * 
 */
UCLASS()
class CMP302_API UCSaveGame : public USaveGame
{
	GENERATED_BODY()

	
public:
	
	UPROPERTY()
	TArray<FActorSaveData> SaveData;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TSubclassOf<UCAction>> PlayerActions;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FTransform> SpawnTransforms;

	UPROPERTY(BlueprintReadOnly)
	int32 Currency = 0;

	UPROPERTY(BlueprintReadOnly)
	FString MapName = TEXT("NoMap");
};
