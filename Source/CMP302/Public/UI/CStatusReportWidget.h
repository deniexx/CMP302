// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CStatusReportWidget.generated.h"

class UCReportEntryWidget;
class UCTutorialWidget;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class CMP302_API UCStatusReportWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void AddStatusMessage(const FString& Message);

	void AddTutorialWidget(const FString& TutorialMessage, const TArray<FString>& KeysToDisplay) const;

protected:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ReportVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* TutorialVerticalBox;

	UPROPERTY()
	TArray<UCReportEntryWidget*> Entries;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	int32 MaxEntries;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UTexture2D* LMBButton;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UTexture2D* RMBButton;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UTexture2D* Mouse;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UCReportEntryWidget> ReportEntryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UCTutorialWidget> TutorialWidgetClass;

};
