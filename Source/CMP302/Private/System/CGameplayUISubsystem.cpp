// Coded by: Denis Dimitrov for CMP302


#include "System/CGameplayUISubsystem.h"

#include "ActorComponents/CActionComponent.h"
#include "Blueprint/UserWidget.h"

UUserWidget* UCGameplayUISubsystem::PushWidget(TSubclassOf<UUserWidget> WidgetClass, bool bAddToViewport)
{
	UUserWidget* Widget = CreateWidget(PlayerController, WidgetClass);
	WidgetStack.Push(Widget);

	if (bAddToViewport)
		Widget->AddToViewport();
	
	return Widget;
}

UUserWidget* UCGameplayUISubsystem::PopWidget(bool bRemoveFromViewport)
{
	if (WidgetStack.Num() <= 0) return nullptr;
	
	UUserWidget* Widget = WidgetStack.Pop();

	if (bRemoveFromViewport)
		Widget->RemoveFromParent();
	
	return Widget;
}

void UCGameplayUISubsystem::SetInputMode(ECInputMode InputMode, bool bInShowMouseCursor, UUserWidget* WidgetToFocus)
{
	UCActionComponent* ActionComponent = UCActionComponent::GetActionComponent(PlayerController->GetPawn());
	switch (InputMode)
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
			if (WidgetToFocus)
				InputModeGameAndUI.SetWidgetToFocus(WidgetToFocus->TakeWidget());
			InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
			PlayerController->SetInputMode(InputModeGameAndUI);
			ActionComponent->ActiveGameplayTags.AddTag(ActionComponent->GetDefaultBlockedTag());
			break;
		}
	}

	PlayerController->SetShowMouseCursor(bInShowMouseCursor);
}

void UCGameplayUISubsystem::BindPlayerController(APlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}

bool UCGameplayUISubsystem::IsWidgetInStack()
{
	return WidgetStack.Num() > 0;
}
