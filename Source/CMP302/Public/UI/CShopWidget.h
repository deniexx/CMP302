// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CShopWidget.generated.h"

class UDataTable;
class UVerticalBox;
class UCShopWidgetComponent;

/**
 * 
 */
UCLASS()
class CMP302_API UCShopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
protected:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ParentVerticalBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCShopWidgetComponent> ShopWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* PurchasableDataTable;

private:

	void PopulateSlots() const;
};
