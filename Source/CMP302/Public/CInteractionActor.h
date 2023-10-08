// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInteractionInterface.h"
#include "CInteractionActor.generated.h"

class USphereComponent;

UCLASS()
class CMP302_API ACInteractionActor : public AActor, public ICInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACInteractionActor();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereCollision;

};
