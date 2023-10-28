// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

class UInputAction;
class UCStatusReportWidget;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class CMP302_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	/** Called when we want to go back in the menu */
	UFUNCTION()
	void BackMenu(const FInputActionValue& Value);

	/** Called when we want to go back in the menu */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCStatusReportWidget> StatusReportWidgetClass;

	/** The HUD widget */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainUIWidget;

	/** The pause menu widget */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidget;

	/** The input action used to back in the menus */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UInputAction* BackInputAction;
};
