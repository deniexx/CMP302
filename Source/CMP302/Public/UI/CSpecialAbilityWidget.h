// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSpecialAbilityWidget.generated.h"

class UCAction;
/**
 * 
 */
UCLASS()
class CMP302_API UCSpecialAbilityWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Binds the action to this widget
	 * @param Action Action to be bound(mainly used for initializing data)
	 */
	UFUNCTION(BlueprintNativeEvent)
	void BindAction(UCAction* Action);

	/**
	 * Selects the widget (play animation, etc...)
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Select();

	/**
	 * Deselects the widget (play animation, etc...)
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Deselect();

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Action")
	UCAction* BoundAction;
};
