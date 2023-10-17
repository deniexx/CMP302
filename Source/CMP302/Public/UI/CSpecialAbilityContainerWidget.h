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

	void AddNewAbilityWidget(UCAction* Action);

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
