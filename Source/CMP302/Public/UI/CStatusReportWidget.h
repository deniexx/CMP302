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
	
	/**
	 * Adds a status message to the status board
	 * @param Message The message to be posted on the status board
	 */
	void AddStatusMessage(const FString& Message);

	/**
	 * Adds a tutorial widget to the screen with the specified message and keys
	 * @param TutorialMessage The tutorial message to be displayed
	 * @param KeysToDisplay The keys required for performing the action to be displayed
	 */
	void AddTutorialWidget(const FString& TutorialMessage, const TArray<FString>& KeysToDisplay) const;

protected:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ReportVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* TutorialVerticalBox;

	UPROPERTY()
	TArray<UCReportEntryWidget*> Entries;

	/** Max entries that can be added to the status board */
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	int32 MaxEntries;

	/** The texture used for Left Mouse Button tutorial icon */
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UTexture2D* LMBButton;

	/** The texture used for Right Mouse Button tutorial icon */
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UTexture2D* RMBButton;

	/** The texture used for the Mouse tutorial icon */
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UTexture2D* Mouse;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UCReportEntryWidget> ReportEntryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UCTutorialWidget> TutorialWidgetClass;

};
