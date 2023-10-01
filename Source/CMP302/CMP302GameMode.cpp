// Copyright Epic Games, Inc. All Rights Reserved.

#include "CMP302GameMode.h"

#include "EngineUtils.h"
#include "Character/CAICharacter.h"
#include "UObject/ConstructorHelpers.h"

ACMP302GameMode::ACMP302GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void ACMP302GameMode::RespawnAllEnemies()
{
	for (TActorIterator<ACAICharacter> It(GetWorld()); It; ++It)
	{
		ACAICharacter* Bot = *It;

		Bot->ReadyActor();
	}
}
