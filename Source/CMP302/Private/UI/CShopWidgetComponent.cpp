// Coded by: Denis Dimitrov for CMP302


#include "UI/CShopWidgetComponent.h"

#include "Actions/CAction.h"
#include "ActorComponents/CActionComponent.h"
#include "Character/CPlayerState.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCShopWidgetComponent::Init(TSubclassOf<UCAction> ActionClass)
{
	Action = ActionClass;

	const UCAction* ActionT = Cast<UCAction>(Action->GetDefaultObject());
	if (ActionT->Icon.IsValid())
	{
		UTexture2D* Texture = ActionT->Icon.LoadSynchronous();
		Icon->SetBrushFromTexture(Texture);
	}

	Name->SetText(ActionT->ActionName);
	Description->SetText(ActionT->ActionDescription);
	Price->SetText(FText::AsNumber(ActionT->Cost));
	Cost = ActionT->Cost;
}

void UCShopWidgetComponent::NativeConstruct()
{
	Super::NativeConstruct();

	Background->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
}

void UCShopWidgetComponent::NativeDestruct()
{
	Super::NativeDestruct();

	Background->OnClicked.RemoveAll(this);
}

void UCShopWidgetComponent::OnClicked()
{
	APawn* OwningPawn = GetOwningPlayerPawn();
	ACPlayerState* PlayerState = OwningPawn->GetPlayerState<ACPlayerState>();
	
	if (PlayerState &&  PlayerState->RemoveCurrency(Cost))
	{
		UCActionComponent* ActionComponent = UCActionComponent::GetActionComponent(OwningPawn);
		ActionComponent->AddAction(OwningPawn, Action);
	}
}
