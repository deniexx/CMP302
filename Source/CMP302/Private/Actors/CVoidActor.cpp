// Coded by: Denis Dimitrov for CMP302


#include "Actors/CVoidActor.h"

// Sets default values
ACVoidActor::ACVoidActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	VoidMode = EVoidMode::None;
}

// Called when the game starts or when spawned
void ACVoidActor::BeginPlay()
{
	Super::BeginPlay();

	OnStateSwitched(VoidMode);
}

void ACVoidActor::SwitchVoidState()
{
	switch (VoidMode)
	{
	case EVoidMode::None: // If we were live or none, go to ghost
	case EVoidMode::Live:
		VoidMode = EVoidMode::Ghost;
		break;
	case EVoidMode::Ghost: // If we were ghost, go to live
		VoidMode = EVoidMode::Live;
		break;
	}

	OnStateSwitched(VoidMode);
}

void ACVoidActor::OnStateSwitched_Implementation(EVoidMode NewVoidMode)
{
	// @NOTE: This must be implemented in blueprints
}
