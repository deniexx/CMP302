// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CMP302GameMode.generated.h"

class UCSaveGame;
class ACRoomManager;
class ACPlayerCharacter;

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

	void AddRoomManager(ACRoomManager* RoomManager);

	void BindToOnHitDelegateForPlayer(ACPlayerCharacter* Player);

private:
	
	void RespawnEnemiesAtRoomIndex(int32 Index);

	void LoadGame();

	UPROPERTY(EditDefaultsOnly)
	bool bIsFirstTutorialLevel;

	UPROPERTY(EditDefaultsOnly)
	FString SaveSlotName;

	UPROPERTY()
	UCSaveGame* CurrentSaveGame;

	UPROPERTY()
	TArray<ACRoomManager*> RoomManagers;
};



