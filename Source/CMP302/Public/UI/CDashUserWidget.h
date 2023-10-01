// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDashUserWidget.generated.h"

class UCAction_Dash;
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
	 * Binds the dash action to this widget, so that we can use the remaining time on the dash here
	 * @param InDashAction  Dash action to be bound to this widget
	 */
	void BindDashAction(UCAction_Dash* InDashAction);
	
protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Parameter collection to be used to drive the UI */
	UPROPERTY(EditDefaultsOnly, Category = "Extras")
	UMaterialParameterCollection* UIParameters;

private:

	UPROPERTY()
	UCAction_Dash* DashAction;
};
