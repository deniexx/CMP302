// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CStatusReportSubsystem.generated.h"

class UCStatusReportWidget;
/**
 * 
 */
UCLASS()
class CMP302_API UCStatusReportSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	/**
	 * Creates the status report widget and sets up the system to be used
	 * @param OwningPlayerController The player controller owning the UI elements
	 * @param StatusReportWidgetClass The status report widget class
	 */
	void InitializeWidget(APlayerController* OwningPlayerController, TSubclassOf<UCStatusReportWidget> StatusReportWidgetClass);

	/**
	 * Display a message on the status report
	 * @param Message Message to display
	 */
	void AddStatusMessage(const FString& Message) const;

	/**
	 * Displays a tutorial message on the screen with the appropriate keys
	 * @param TutorialMessage Tutorial message to be shown
	 * @param KeysToDisplay Keys associated to be displayed
	 */
	void AddTutorialMessage(const FString& TutorialMessage, const TArray<FString>& KeysToDisplay) const;


private:
	
	UPROPERTY()
	UCStatusReportWidget* StatusReportWidget;
};
