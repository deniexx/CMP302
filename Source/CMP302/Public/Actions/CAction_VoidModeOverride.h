// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_VoidModeOverride.generated.h"

class ACCommonCharacter;
class UInputAction;
class ACVoidActor;
/**
 * 
 */
UCLASS()
class CMP302_API UCAction_VoidModeOverride : public UCAction
{
	GENERATED_BODY()

public:

	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;

	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

	virtual void StartAction_Implementation(AActor* InInstigator) override;

protected:

	/** The input action used to swap void mode */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InputAction;

	UPROPERTY()
	int32 InputBindingHandle;
	
private:

	UPROPERTY()
	ACCommonCharacter* Character;

	UFUNCTION()
	void ActivateActionOnPress();

	UPROPERTY()
	TArray<ACVoidActor*> VoidActors;
	
};
