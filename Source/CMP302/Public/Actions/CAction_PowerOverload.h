// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_PowerOverload.generated.h"

class UCOverloadActionWidget;
class ACCommonCharacter;

/**
 * 
 */
UCLASS()
class CMP302_API UCAction_PowerOverload : public UCAction
{
	GENERATED_BODY()

public:

	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;

	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

	virtual void TickAction_Implementation(float DeltaTime) override;

	virtual bool CanStart_Implementation(AActor* InInstigator) override;

	virtual void StartAction_Implementation(AActor* InInstigator) override;

	virtual void StopAction_Implementation(AActor* InInstigator) override;

	/** Adds a charge to the action, making you able to use it again */
	UFUNCTION(BlueprintCallable)
	void AddCharge();
	
protected:

	/** The duration of the overload action */
	UPROPERTY(EditDefaultsOnly)
	float OverloadDuration;

	/** The widget class to display */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCOverloadActionWidget> OverloadActionWidgetClass;

	UPROPERTY()
	UCOverloadActionWidget* OverloadActionWidgetInstance;

	/** This will drive ui material parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UMaterialParameterCollection* UIMaterialParameters;

	UPROPERTY()
	ACCommonCharacter* Character;

private:

	/** Time elapsed of the overload action */
	float TimeElapsed = 0.f;

	/** Whether we have a charge or not */
	bool bHasCharge = true;
};
