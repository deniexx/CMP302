// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CMP302GameMode.generated.h"

class UCSaveGame;

UCLASS(minimalapi)
class ACMP302GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ACMP302GameMode();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RespawnAllEnemies();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UCSaveGame* GetSaveGame() const;

	void WriteSaveGame();

private:
	
	void LoadGame();

	UPROPERTY(EditDefaultsOnly)
	bool bIsFirstTutorialLevel;

	UPROPERTY(EditDefaultsOnly)
	FString SaveSlotName;

	UPROPERTY()
	UCSaveGame* CurrentSaveGame;
};



