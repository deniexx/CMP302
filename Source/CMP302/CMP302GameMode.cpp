// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP302GameMode.h"

#include "EngineUtils.h"
#include "Character/CAICharacter.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "System/CSaveGame.h"
#include "UObject/ConstructorHelpers.h"
#include "CRoomManager.h"
#include "Character/CPlayerCharacter.h"
#include "Character/CPlayerState.h"

ACMP302GameMode::ACMP302GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	SaveSlotName = TEXT("DefaultSave");
	bIsFirstTutorialLevel = false;
}

void ACMP302GameMode::RespawnAllEnemies()
{
	for (TActorIterator<ACAICharacter> It(GetWorld()); It; ++It)
	{
		ACAICharacter* Bot = *It;

		Bot->ReadyActor();
	}
}

void ACMP302GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadGame();
}

void ACMP302GameMode::RespawnEnemiesAtRoomIndex(int32 Index)
{
	//RoomManagers[Index]->SpawnEnemies();
}

void ACMP302GameMode::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0) && !bIsFirstTutorialLevel)
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
	}
}

UCSaveGame* ACMP302GameMode::GetSaveGame() const
{
	return CurrentSaveGame;
}

void ACMP302GameMode::WriteSaveGame()
{
	CurrentSaveGame->SaveData.Empty();
	
	for (const ACRoomManager* RoomManager : RoomManagers)
	{
		FActorSaveData SaveData;
		SaveData.ActorName = RoomManager->GetName();
		SaveData.bCleared = RoomManager->GetIsCleared();
		CurrentSaveGame->SaveData.Add(SaveData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

void ACMP302GameMode::AddRoomManager(ACRoomManager* RoomManager)
{
	if (RoomManager)
	{
		RoomManager->SetRoomIndex(RoomManagers.AddUnique(RoomManager));
	
		for (const FActorSaveData& SaveData : CurrentSaveGame->SaveData)
		{
			if (SaveData.ActorName == RoomManager->GetName())
			{
				RoomManager->SetIsCleared(SaveData.bCleared);
				break;
			}
		}
	}
}

void ACMP302GameMode::BindToOnHitDelegateForPlayer(ACPlayerCharacter* Player)
{
	if (Player)
		Player->OnPlayerHit.AddDynamic(this, &ThisClass::RespawnEnemiesAtRoomIndex);
}
