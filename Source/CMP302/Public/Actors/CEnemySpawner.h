// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "GameFramework/Actor.h"
#include "CEnemySpawner.generated.h"

class ACRoomManager;
class UArrowComponent;

UCLASS()
class CMP302_API ACEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACEnemySpawner();

	/** Registers the Spawner to the room manager */
	void Init(ACRoomManager* InRoomManager);

	/** Spawns an enemy */
	int32 SpawnEnemy();

	/** Kills the spawned enemy if there is one */
	void KillEnemy();
	
protected:

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AActor> EnemyToSpawn;

	UPROPERTY()
	ACRoomManager* RoomManager;

private:

	UFUNCTION()
	void OnHit(const FAttackData& AttackData);

	UPROPERTY()
	AActor* SpawnedEnemy;
};
