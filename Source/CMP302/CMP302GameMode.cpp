// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP302GameMode.h"

#include "EngineUtils.h"
#include "Character/CAICharacter.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "System/CSaveGame.h"
#include "UObject/ConstructorHelpers.h"

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

UCSaveGame* ACMP302GameMode::GetSaveGame() const
{
	return CurrentSaveGame;
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

void ACMP302GameMode::WriteSaveGame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}
