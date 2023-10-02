// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CStatusReportWidget.generated.h"

class UCReportEntryWidget;
class UImage;
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

	void UpdateStatusImage(UTexture2D* StatusTexture) const;

protected:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ReportVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UImage* StatusImage;

	UPROPERTY()
	TArray<UCReportEntryWidget*> Entries;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	int32 MaxEntries;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UCReportEntryWidget> ReportEntryWidget;
};
