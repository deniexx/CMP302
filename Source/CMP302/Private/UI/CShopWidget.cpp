// Coded by: Denis Dimitrov for CMP302


#include "UI/CShopWidget.h"

#include "Components/VerticalBox.h"
#include "UI/CShopWidgetComponent.h"

#include "Engine/DataTable.h"
#include "Extras/CDataTableRow.h"

struct FCPurchasableDataTableRow;

void UCShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PopulateSlots();
}

void UCShopWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ParentVerticalBox->ClearChildren();
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
