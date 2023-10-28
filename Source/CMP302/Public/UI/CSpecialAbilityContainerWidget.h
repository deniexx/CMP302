// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSpecialAbilityContainerWidget.generated.h"

class UVerticalBox;
class UCSpecialAbilityWidget;
class UCAction;
/**
 * 
 */
UCLASS()
class CMP302_API UCSpecialAbilityContainerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Adds a new action/ability to be available for selection
	 * @param Action Action to be added as a selection
	 */
	void AddNewAbilityWidget(UCAction* Action);

	/**
	 * Updates the active(selected) action
	 * @param Index The index of the currently selected action
	 * @param PreviousIndex The index of the previously selected action
	 */
	void UpdateActiveAction(int32 Index, int32 PreviousIndex);

protected:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ParentVerticalBox;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSubclassOf<UCSpecialAbilityWidget> SpecialAbilityWidgetClass;

private:

	UPROPERTY()
	TArray<UCSpecialAbilityWidget*> SpecialAbilityWidgets;
	
};
