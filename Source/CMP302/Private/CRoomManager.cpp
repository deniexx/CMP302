// Coded by: Denis Dimitrov for CMP302


#include "CRoomManager.h"

// Sets default values
ACRoomManager::ACRoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RoomIndex = 0;
}

// Called when the game starts or when spawned
void ACRoomManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACRoomManager::SetRoomIndex(int32 Index)
{
	RoomIndex = Index;
}

