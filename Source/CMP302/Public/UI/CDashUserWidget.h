// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDashUserWidget.generated.h"

class UCCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class CMP302_API UCDashUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * Binds the movement component to this widget, so that we can use the remaining time on the dash here
	 * @param InMovementComponent Binds the movement component to this widget
	 */
	void BindMovementComponent(UCCharacterMovementComponent* InMovementComponent);
	
protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Parameter collection to be used to drive the UI */
	UPROPERTY(EditDefaultsOnly, Category = "Extras")
	UMaterialParameterCollection* UIParameters;

private:

	UPROPERTY()
	UCCharacterMovementComponent* MovementComponent;
};
