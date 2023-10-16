// Coded by: Denis Dimitrov for CMP302


#include "System/CGameplayUISubsystem.h"

#include "ActorComponents/CActionComponent.h"
#include "Blueprint/UserWidget.h"

UUserWidget* UCGameplayUISubsystem::PushWidget(TSubclassOf<UUserWidget> WidgetClass, ECInputMode InputMode, bool bShowMouseCursor, bool bFocusWidget, bool bPause)
{
	UUserWidget* Widget = CreateWidget(PlayerController, WidgetClass);
	FGameplayUIData GameplayUIData;
	GameplayUIData.InputMode = InputMode;
	GameplayUIData.bShowMouseCursor= bShowMouseCursor;
	GameplayUIData.Widget = Widget;
	GameplayUIData.bFocusWidget = bFocusWidget;
	GameplayUIData.bPaused = bPause;
	
	WidgetStack.Push(GameplayUIData);
	
	Widget->AddToViewport();
	SetInputMode(GameplayUIData);
	
	return Widget;
}

bool UCGameplayUISubsystem::PopWidget()
{
	if (WidgetStack.Num() <= 0) return false; // No widgets were in the stack, so we can not remove any

	const FGameplayUIData Data = WidgetStack.Pop();
	Data.Widget->RemoveFromParent();
	
	if (WidgetStack.Num() > 0)
	{
		SetInputMode(WidgetStack.Last());
	}
	else
	{
		// If we have no elements after the pop, we reset to the default input mode (GameOnly, don't show mouse cursor)
		FGameplayUIData GameplayUIData;
		GameplayUIData.InputMode = ECInputMode::GameOnly;
		GameplayUIData.bShowMouseCursor = false;
		GameplayUIData.bFocusWidget = false;
		GameplayUIData.bPaused = false;

		SetInputMode(GameplayUIData);
	}

	return true; // Widget has been removed from the stack
}

void UCGameplayUISubsystem::SetInputMode(const FGameplayUIData& Data)
{
	UCActionComponent* ActionComponent = UCActionComponent::GetActionComponent(PlayerController->GetPawn());
	switch (Data.InputMode)
	{
		case ECInputMode::GameOnly:
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
			ActionComponent->ActiveGameplayTags.RemoveTag(ActionComponent->GetDefaultBlockedTag());
			break;
		}
		case ECInputMode::UIOnly: // This is UI Only!
		{
			FInputModeGameAndUI InputModeGameAndUI;
			if (Data.bFocusWidget)
				InputModeGameAndUI.SetWidgetToFocus(Data.Widget->TakeWidget());
			InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
			PlayerController->SetInputMode(InputModeGameAndUI);
			ActionComponent->ActiveGameplayTags.AddTag(ActionComponent->GetDefaultBlockedTag());
			break;
		}
	}

	PlayerController->SetPause(Data.bPaused);
	PlayerController->SetShowMouseCursor(Data.bShowMouseCursor);
}

void UCGameplayUISubsystem::BindPlayerController(APlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}

bool UCGameplayUISubsystem::IsWidgetInStack()
{
	return WidgetStack.Num() > 0;
}
