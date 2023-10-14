// Coded by: Denis Dimitrov for CMP302


#include "Actors/CEnemySpawner.h"

#include "CRoomManager.h"
#include "ActorComponents/CCombatStatusComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACEnemySpawner::ACEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
}

void ACEnemySpawner::Init(ACRoomManager* InRoomManager)
{
	RoomManager = InRoomManager;
}

int32 ACEnemySpawner::SpawnEnemy()
{
	if (IsValid(EnemyToSpawn) && !SpawnedEnemy)
	{
		const FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnedEnemy = GetWorld()->SpawnActor(EnemyToSpawn, &SpawnTransform, SpawnParameters);

		UCCombatStatusComponent::GetCombatStatusComponent(SpawnedEnemy)->OnHitTaken.AddDynamic(this, &ThisClass::OnHit);
		return 1;
	}
	
	return 0;
}

void ACEnemySpawner::KillEnemy()
{
	if (SpawnedEnemy)
	{
		SpawnedEnemy->Destroy();
		SpawnedEnemy = nullptr;
	}
}

void ACEnemySpawner::OnHit(const FAttackData& AttackData)
{
	SpawnedEnemy = nullptr;
	RoomManager->EnemyKilled();
}

