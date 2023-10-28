// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRoomManager.generated.h"

class UBoxComponent;
class ACEnemySpawner;

UCLASS()
class CMP302_API ACRoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACRoomManager();

	/** Event called by the enemy when it has been killed */
	void EnemyKilled();

	/**
	 * Checks if the room has been cleared or not
	 * @return Whether the room has been cleared
	 */
	bool GetIsCleared() const;

	/**
	 * Sets whether the room has been cleared (currently used for debugging and loading)
	 * @param bNewState The new state to be set
	 */
	void SetIsCleared(bool bNewState);

	/**
	 * Resets the state of the room (kills all enemies in it)
	 */
	void ResetRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Called when the room has been cleared, saves the cleared state in the save object
	 */
	void RoomCleared();

	/**
	 * Gets the enemy spawners to spawn enemies in the room
	 */
	void SpawnEnemies();

	/**
	 * Kills all the enemies in the room
	 */
	void KillEnemies();

	/** The index of the room */
	uint32 RoomIndex;

	/** The number of remaining enemies in the room */
	uint32 RemainingEnemies;

	/** Whether the room has been cleared */
	bool bRoomCleared;

	/** The enemy spawners assigned to this room */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<ACEnemySpawner*> EnemySpawners;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* RoomExtends;

	UFUNCTION()
	void OnRoomEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

public:
	/**
	 * Sets the index of the current room to the given value
	 * @param Index The new index of the room be set
	 */
	void SetRoomIndex(int32 Index);

};
