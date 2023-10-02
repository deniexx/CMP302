// Coded by: Denis Dimitrov for CMP302


#include "System/CStatusReportSubsystem.h"

#include "UI/CStatusReportWidget.h"

void UCStatusReportSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	
}

void UCStatusReportSubsystem::Deinitialize()
{
	Super::Deinitialize();

	
}

void UCStatusReportSubsystem::InitializeWidget(APlayerController* OwningPlayerController,
	TSubclassOf<UCStatusReportWidget> StatusReportWidgetClass)
{
	if (!OwningPlayerController) return;
	if (!IsValid(StatusReportWidgetClass)) return;

	StatusReportWidget = CreateWidget<UCStatusReportWidget>(OwningPlayerController, StatusReportWidgetClass);
	StatusReportWidget->AddToViewport();
}

void UCStatusReportSubsystem::AddStatusMessage(const FString& Message) const
{
	if (StatusReportWidget)
		StatusReportWidget->AddStatusMessage(Message);
}

void UCStatusReportSubsystem::UpdateStatusImage(UTexture2D* StatusTexture) const
{
	if (StatusReportWidget)
		StatusReportWidget->UpdateStatusImage(StatusTexture);
}
