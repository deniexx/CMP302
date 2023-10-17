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

	UFUNCTION(BlueprintNativeEvent)
	void BindAction(UCAction* Action);

	UFUNCTION(BlueprintNativeEvent)
	void Select();

	UFUNCTION(BlueprintNativeEvent)
	void Deselect();

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Action")
	UCAction* BoundAction;
};
