// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CShopWidgetComponent.generated.h"

class UButton;
class UTextBlock;
class UCAction;
class UImage;

/**
 * 
 */
UCLASS()
class CMP302_API UCShopWidgetComponent : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Initializes the widget with the required data from the action
	 * @param ActionClass Action class to be used to take data from
	 */
	void Init(TSubclassOf<UCAction> ActionClass);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
protected:

	UPROPERTY(meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	UImage* DisabledOverlay;

	UPROPERTY(meta = (BindWidget))
	UButton* Background;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Price;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<USoundBase> TransactionSuccessfulSound;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<USoundBase> TransactionFailedSound;
	
private:

	int32 Cost;
	
	UFUNCTION()
	void OnClicked();

	UPROPERTY()
	TSubclassOf<UCAction> Action;
};
