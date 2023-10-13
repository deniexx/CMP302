// Coded by: Denis Dimitrov for CMP302


#include "UI/CShopWidget.h"

#include "Character/CPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/CShopWidgetComponent.h"

#include "Engine/DataTable.h"
#include "Extras/CDataTableRow.h"

struct FCPurchasableDataTableRow;

void UCShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PopulateSlots();

	ACPlayerState* PlayerState = GetOwningPlayer()->GetPlayerState<ACPlayerState>();
	PlayerState->OnCurrencyUpdated.AddDynamic(this, &ThisClass::OnCurrencyUpdated);
	CurrencyAmount->SetText(FText::AsNumber(PlayerState->GetCurrency()));
}

void UCShopWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ParentVerticalBox->ClearChildren();
	
	ACPlayerState* PlayerState = GetOwningPlayer()->GetPlayerState<ACPlayerState>();
    PlayerState->OnCurrencyUpdated.RemoveDynamic(this, &ThisClass::OnCurrencyUpdated);
}

void UCShopWidget::OnCurrencyUpdated(int32 NewAmount, int32 OldAmount)
{
	const UGameInstance* GameInstance = GetGameInstance();
	UTweenSubsystem* TweenSubsystem = GameInstance ? GameInstance->GetSubsystem<UTweenSubsystem>() : nullptr;

	if (!TweenSubsystem)
	{
		CurrencyAmount->SetText(FText::AsNumber(NewAmount));
		return;
	}

	TargetCurrency = NewAmount;
	if (TweenHandle.IsValid())
	{
		// If we are currently tweening, just update the target amount
		return;
	}

	StartingCurrency = OldAmount;
	FTweenDelegate TweenDelegate;
	TweenDelegate.BindUObject(this, &ThisClass::TweenCurrency);
	TweenSubsystem->AddTween(TweenHandle, 0, 1, TweenDelegate);
}

void UCShopWidget::PopulateSlots() const
{
	TArray<FCPurchasableDataTableRow*> PurchasableRows;

	PurchasableDataTable->GetAllRows<FCPurchasableDataTableRow>("", PurchasableRows);

	if (PurchasableRows.Num() > 0)
	{
		for (const FCPurchasableDataTableRow* Row : PurchasableRows)
		{
			APlayerController* PlayerController = GetOwningPlayerPawn()->GetController<APlayerController>();
			UCShopWidgetComponent* Component = CreateWidget<UCShopWidgetComponent>(PlayerController, ShopWidgetComponent);
			Component->Init(Row->ActionClass);
			ParentVerticalBox->AddChildToVerticalBox(Component);
		}
	}
}

void UCShopWidget::TweenCurrency(float Value)
{
	const int32 Amount = FMath::InterpEaseOut(TargetCurrency, StartingCurrency,  1 - Value, 1);
	CurrencyAmount->SetText(FText::AsNumber(Amount));
}
