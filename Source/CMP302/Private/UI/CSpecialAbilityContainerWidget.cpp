// Coded by: Denis Dimitrov for CMP302


#include "UI/CSpecialAbilityContainerWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "UI/CSpecialAbilityWidget.h"

void UCSpecialAbilityContainerWidget::AddNewAbilityWidget(UCAction* Action)
{
	UCSpecialAbilityWidget* NewWidget = CreateWidget<UCSpecialAbilityWidget>(GetOwningPlayer(), SpecialAbilityWidgetClass);
	NewWidget->BindAction(Action);
	const int32 Index = SpecialAbilityWidgets.Add(NewWidget);

	/* If first widget added select it, else deselect it */
	Index == 0 ? NewWidget->Select() : NewWidget->Deselect();
	
	ParentVerticalBox->AddChildToVerticalBox(NewWidget);
	USpacer* Spacer = WidgetTree->ConstructWidget<USpacer>();
	Spacer->SetSize(FVector2d(0.f, 5.f));
	ParentVerticalBox->AddChildToVerticalBox(Spacer);
}

void UCSpecialAbilityContainerWidget::UpdateActiveAction(int32 Index, int32 PreviousIndex)
{
	if (SpecialAbilityWidgets.Num() == 0) return;
	
	SpecialAbilityWidgets[PreviousIndex]->Deselect();
	SpecialAbilityWidgets[Index]->Select();
}
