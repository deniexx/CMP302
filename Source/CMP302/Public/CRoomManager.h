// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRoomManager.generated.h"

UCLASS()
class CMP302_API ACRoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACRoomManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int32 RoomIndex;

public:

	void SetRoomIndex(int32 Index);

};
