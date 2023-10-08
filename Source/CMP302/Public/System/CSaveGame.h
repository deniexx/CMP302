// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CSaveGame.generated.h"

class UCAction;
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
};
