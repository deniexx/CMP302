// Coded by: Denis Dimitrov for CMP302


#include "CInteractionActor.h"

#include "Components/SphereComponent.h"

// Sets default values
ACInteractionActor::ACInteractionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
}
