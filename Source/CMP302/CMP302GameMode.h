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

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/**
	 * Gets the save game object for the current game mode
	 * @return The save game object for this game mode
	 */
	UCSaveGame* GetSaveGame() const;

	/** Writes to the save game the current state of the game */
	void WriteSaveGame();

	/**
	 * Adds a room manager to the array (this will also assign it an index)
	 * @param RoomManager The room manager to append to the array of room managers
	 */
	void AddRoomManager(ACRoomManager* RoomManager);

	/**
	 * Binds an event to the on hit delegate from the player
	 * @param Player The player character to be used to get the on hit delegate from
	 */
	void BindToOnHitDelegateForPlayer(ACPlayerCharacter* Player);

protected:
	
	/**
	 * Resets the save game to its default values
	 */
	UFUNCTION(BlueprintCallable)
	void ResetSaveGame();

	/** The name of the save slot currently used */
	UPROPERTY(EditDefaultsOnly)
	FString SaveSlotName;

	/** The death widget class to be used as a template to show to the player */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DeathWidgetClass; 

	/** Whether the player is currently alive */
	bool bPlayerAlive;
	
#if WITH_EDITOR
	UFUNCTION(Exec)
	void RestartRooms();
#endif

private:

	/** Loads the save game from the specified slot name */
	void LoadGame();

	UFUNCTION()
	void OnPlayerHit(int RoomIndex);

	/** The instance of the current save game object */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UCSaveGame* CurrentSaveGame;

	/** The array of room managers in the level */
	UPROPERTY()
	TArray<ACRoomManager*> RoomManagers;
};



