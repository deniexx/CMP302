// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "System/TweenSubsystem.h"
#include "CShopWidget.generated.h"

class UScrollBox;
class UDataTable;
class UVerticalBox;
class UTextBlock;
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

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ShopScrollBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrencyAmount;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCShopWidgetComponent> ShopWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* PurchasableDataTable;

	int32 Currency;

	UFUNCTION()
	void OnCurrencyUpdated(int32 NewAmount, int32 OldAmount);

	FTweenHandle TweenHandle;

private:

	void PopulateSlots() const;

	UFUNCTION()
	void TweenCurrency(float Value);

	uint32 StartingCurrency;
	uint32 TargetCurrency;

	/* Used to track the interpolated value, so that we don't get jumps on a new purchase */
	uint32 CurrentCurrency;
};
