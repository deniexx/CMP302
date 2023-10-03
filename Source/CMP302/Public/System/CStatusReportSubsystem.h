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

	void InitializeWidget(APlayerController* OwningPlayerController, TSubclassOf<UCStatusReportWidget> StatusReportWidgetClass);

	void AddStatusMessage(const FString& Message) const;

	void AddTutorialMessage(const FString& TutorialMessage, const TArray<FString>& KeysToDisplay) const;


private:
	
	UPROPERTY()
	UCStatusReportWidget* StatusReportWidget;
};
