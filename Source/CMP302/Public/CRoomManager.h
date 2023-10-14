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
	
	void EnemyKilled();

	bool GetIsCleared() const;

	void SetIsCleared(bool bNewState);

	void ResetRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RoomCleared();

	void SpawnEnemies();

	void KillEnemies();

	uint32 RoomIndex;

	uint32 RemainingEnemies;
	
	bool bRoomCleared;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<ACEnemySpawner*> EnemySpawners;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* RoomExtends;

	UFUNCTION()
	void OnRoomEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

public:

	void SetRoomIndex(int32 Index);

};
