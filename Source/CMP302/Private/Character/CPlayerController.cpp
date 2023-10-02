// Coded by: Denis Dimitrov for CMP302


#include "Character/CPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "System/CStatusReportSubsystem.h"

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const UGameInstance* GameInstance = GetGameInstance();
	UCStatusReportSubsystem* StatusReportSubsystem = GameInstance ? GameInstance->GetSubsystem<UCStatusReportSubsystem>() : nullptr;
	StatusReportSubsystem->InitializeWidget(this, StatusReportWidgetClass);

	UUserWidget* MainUI = CreateWidget(this, MainUIWidget);
	MainUI->AddToViewport();
}
