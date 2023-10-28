// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "COverloadActionWidget.generated.h"

class UCAction_PowerOverload;
/**
 * 
 */
UCLASS()
class CMP302_API UCOverloadActionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void BindPowerOverloadAction(UCAction_PowerOverload* InPowerOverloadAction);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateChargeReceived();

protected:

	UPROPERTY(BlueprintReadOnly)
	UCAction_PowerOverload* PowerOverloadAction;
};
