// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTutorialWidget.generated.h"

class UHorizontalBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class CMP302_API UCTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* KeysHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TutorialText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KeysText;
};
