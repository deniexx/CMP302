// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP302GameMode.h"

#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "System/CSaveGame.h"
#include "UObject/ConstructorHelpers.h"
#include "CRoomManager.h"
#include "Character/CPlayerCharacter.h"
#include "System/CGameplayUISubsystem.h"

ACMP302GameMode::ACMP302GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	SaveSlotName = TEXT("DefaultSave");
}

void ACMP302GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadGame();
}

void ACMP302GameMode::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
	}
}

void ACMP302GameMode::OnPlayerHit(int RoomIndex)
{
	if (RoomIndex < RoomManagers.Num())
	{
		RoomManagers[RoomIndex]->ResetRoom();
	}

	UCGameplayUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UCGameplayUISubsystem>();

	UISubsystem->PushWidget(DeathWidgetClass, ECInputMode::UIOnly, true, true, true);
}

UCSaveGame* ACMP302GameMode::GetSaveGame() const
{
	return CurrentSaveGame;
}

void ACMP302GameMode::WriteSaveGame()
{
	CurrentSaveGame->SaveData.Empty();

	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	for (const ACRoomManager* RoomManager : RoomManagers)
	{
		FActorSaveData SaveData;
		SaveData.ActorName = LevelName.Append(RoomManager->GetName());
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

		FString ToCheck = GetWorld()->GetMapName();
		ToCheck.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		ToCheck.Append(RoomManager->GetName());
		
		for (const FActorSaveData& SaveData : CurrentSaveGame->SaveData)
		{
			if (SaveData.ActorName == ToCheck)
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
		Player->OnPlayerHit.AddDynamic(this, &ThisClass::OnPlayerHit);
}

void ACMP302GameMode::ResetSaveGame()
{
	CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}

#if WITH_EDITOR
void ACMP302GameMode::RestartRooms()
{
	for (ACRoomManager* RoomManager : RoomManagers)
	{
		RoomManager->SetIsCleared(false);
	}
}
#endif
