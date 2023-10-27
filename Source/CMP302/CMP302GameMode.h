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

	UCSaveGame* GetSaveGame() const;

	void WriteSaveGame();

	void AddRoomManager(ACRoomManager* RoomManager);
	
	void BindToOnHitDelegateForPlayer(ACPlayerCharacter* Player);

protected:

	UFUNCTION(BlueprintCallable)
	void ResetSaveGame();

	UPROPERTY(EditDefaultsOnly)
	FString SaveSlotName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DeathWidgetClass; 

#if WITH_EDITOR
	UFUNCTION(Exec)
	void RestartRooms();
#endif

private:

	void LoadGame();

	UFUNCTION()
	void OnPlayerHit(int RoomIndex);

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UCSaveGame* CurrentSaveGame;
	
	UPROPERTY()
	TArray<ACRoomManager*> RoomManagers;
};



