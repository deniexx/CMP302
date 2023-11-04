// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "GameFramework/Actor.h"
#include "CVoidActor.generated.h"

UCLASS()
class CMP302_API ACVoidActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACVoidActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** This will be the starting void mode, Live = Enabled, Ghost = Disabled */
	UPROPERTY(EditAnywhere, Category = "VoidMode")
	EVoidMode VoidMode;

	UFUNCTION(BlueprintNativeEvent)
	void OnStateSwitched(EVoidMode NewVoidMode);

public:
	
	/**
	 * Switches the void state of the current actor from Live to Ghost or vice versa
	 */
	void SwitchVoidState();

};
