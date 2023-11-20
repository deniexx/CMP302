// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CGameplayUISubsystem.generated.h"

class UUserWidget;

USTRUCT()
struct FGameplayUIData
{
	GENERATED_BODY()

	UPROPERTY()
	ECInputMode InputMode = ECInputMode::None;

	UPROPERTY()
	bool bShowMouseCursor = false;

	UPROPERTY()
	bool bFocusWidget = false;

	UPROPERTY()
	bool bPaused = false;

	UPROPERTY()
	UUserWidget* Widget;
};

/**
 * 
 */
UCLASS()
class CMP302_API UCGameplayUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Creates a widget and pushes it to the widget stack
	 * @param WidgetClass The widget class to use as a template
	 * @param InputMode Input mode to set to
	 * @param bShowMouseCursor Whether cursor should be shown
	 * @param bFocusWidget Whether the new widget should be focused
	 * @param bPause Whether the game should be paused
	 * @return The newly created widget
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayUI")
	UUserWidget* PushWidget(TSubclassOf<UUserWidget> WidgetClass, ECInputMode InputMode = ECInputMode::None, bool bShowMouseCursor = false, bool bFocusWidget = false, bool bPause = false);

	/**
	 * Pops a widget from the widget stack and removes it from the viewport, as well as restoring state to the previous widget state
	 * @return Whether a widget was removed (if one has not been removed, means that the stack was empty)
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayUI")
	bool PopWidget();

	/**
	 * Checks if there are any widgets in the stack
	 * @return Whether we have a widget in the stacks
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayUI")
	bool IsWidgetInStack();

	/**
	 * Binds the player controller to this subsystem (meaning that widgets will be added to that controller)
	 * @param InPlayerController PlayerController to bind to this subsystem
	 */
	void BindPlayerController(APlayerController* InPlayerController);
	
protected:

	UPROPERTY()
	TArray<FGameplayUIData> WidgetStack;

private:

	UPROPERTY()
	APlayerController* PlayerController;

	/**
	 * Sets the input mode appropriately based on given GameplayUI Data
	 * @param Data GameplayUI data to be used to determine the input mode
	 */
	void SetInputMode(const FGameplayUIData& Data);
};
