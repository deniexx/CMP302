// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CReportEntryWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class CMP302_API UCReportEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock;
};
