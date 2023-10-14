// Coded by: Denis Dimitrov for CMP302


#include "CRoomManager.h"

#include "Actors/CEnemySpawner.h"
#include "Character/CPlayerCharacter.h"
#include "CMP302/CMP302GameMode.h"
#include "Components/BoxComponent.h"

// Sets default values
ACRoomManager::ACRoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RoomExtends = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomExtends"));
	RoomExtends->SetupAttachment(RootComponent);
	RoomExtends->SetCollisionResponseToAllChannels(ECR_Ignore);
	RoomExtends->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	RoomIndex = 0;
	bRoomCleared = false;
	RemainingEnemies = 0;
}

// Called when the game starts or when spawned
void ACRoomManager::BeginPlay()
{
	Super::BeginPlay();

	RoomExtends->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnRoomEntered);

	for (ACEnemySpawner* Spawner : EnemySpawners)
	{
		// Initialize spawners to have a reference to their room manager
		Spawner->Init(this);
	}

	ACMP302GameMode* GameMode = GetWorld()->GetAuthGameMode<ACMP302GameMode>();
	GameMode->AddRoomManager(this);
}

void ACRoomManager::SpawnEnemies()
{
	RemainingEnemies = 0;
	for (ACEnemySpawner* Spawner : EnemySpawners)
	{
		RemainingEnemies += Spawner->SpawnEnemy();
	}
}

void ACRoomManager::KillEnemies()
{
	for (ACEnemySpawner* Spawner : EnemySpawners)
	{
		Spawner->KillEnemy();
	}
}

bool ACRoomManager::GetIsCleared() const
{
	return bRoomCleared;
}

void ACRoomManager::SetIsCleared(bool bNewState)
{
	bRoomCleared = bNewState;
}

void ACRoomManager::ResetRoom()
{
	KillEnemies();
	SpawnEnemies();
}

void ACRoomManager::EnemyKilled()
{
	--RemainingEnemies;
	if (RemainingEnemies == 0)
		RoomCleared();
}

void ACRoomManager::RoomCleared()
{
	bRoomCleared = true;
	RoomExtends->OnComponentBeginOverlap.RemoveAll(this);

	ACMP302GameMode* GameMode = GetWorld()->GetAuthGameMode<ACMP302GameMode>();
	GameMode->WriteSaveGame();
}

void ACRoomManager::SetRoomIndex(int32 Index)
{
	RoomIndex = Index;
}

void ACRoomManager::OnRoomEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bRoomCleared) return;
	
	if (OtherActor->IsA(ACPlayerCharacter::StaticClass()))
	{
		SpawnEnemies();
	}
}
