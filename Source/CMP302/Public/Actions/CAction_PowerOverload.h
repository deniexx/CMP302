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
	
	UFUNCTION(BlueprintCallable)
	void AddCharge();
	
protected:

	UPROPERTY(EditDefaultsOnly)
	float OverloadDuration;

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

	bool bHasCharge;
};
