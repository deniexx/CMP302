// Coded by: Denis Dimitrov for CMP302


#include "Character/CPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "System/CGameplayUISubsystem.h"
#include "System/CStatusReportSubsystem.h"

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(BackInputAction, ETriggerEvent::Started, this, &ThisClass::BackMenu);
	}
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const UGameInstance* GameInstance = GetGameInstance();
	
	if (UCStatusReportSubsystem* StatusReportSubsystem = GameInstance ? GameInstance->GetSubsystem<UCStatusReportSubsystem>() : nullptr)
		StatusReportSubsystem->InitializeWidget(this, StatusReportWidgetClass);

	if (UCGameplayUISubsystem* GameplayUISubsystem = GameInstance ? GameInstance->GetSubsystem<UCGameplayUISubsystem>() : nullptr)
		GameplayUISubsystem->BindPlayerController(this);

	UUserWidget* MainUI = CreateWidget(this, MainUIWidget);
	MainUI->AddToViewport();
}

void ACPlayerController::BackMenu(const FInputActionValue& Value)
{
	const UGameInstance* GameInstance = GetGameInstance();

	if (UCGameplayUISubsystem* GameplayUISubsystem = GameInstance ? GameInstance->GetSubsystem<UCGameplayUISubsystem>() : nullptr)
	{
		if (GameplayUISubsystem->PopWidget())
		{
			// We can do stuff here if we want to change something after we pop, but we still have widgets in the stack
		}
		else
		{
			GameplayUISubsystem->PushWidget(PauseMenuWidget, ECInputMode::UIOnly, true, true, true);
		}
	}

}
