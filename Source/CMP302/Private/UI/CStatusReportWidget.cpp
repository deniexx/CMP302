// Coded by: Denis Dimitrov for CMP302


#include "UI/CStatusReportWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "UI/CTutorialWidget.h"
#include "UI/CReportEntryWidget.h"

void UCStatusReportWidget::AddStatusMessage(const FString& Message)
{
	if (Entries.Num() > 0)
	{
		if (const UCReportEntryWidget* EntryWidget = Entries[Entries.Num() - 1])
		{
			if (EntryWidget->TextBlock->GetText().EqualTo(FText::FromString(Message)))
			{
				return;
			}
		}
	}
	
	if (Entries.Num() > MaxEntries)
	{
		Entries[0]->RemoveFromParent();
		Entries.RemoveAt(0);
	}

	UCReportEntryWidget* ReportEntry = Entries.Add_GetRef(CreateWidget<UCReportEntryWidget>(GetOwningPlayer(), ReportEntryWidget));
	ReportEntry->TextBlock->SetText(FText::FromString(Message));
	ReportVerticalBox->AddChildToVerticalBox(ReportEntry);
}

void UCStatusReportWidget::AddTutorialWidget(const FString& TutorialMessage, const TArray<FString>& KeysToDisplay) const
{
	UCTutorialWidget* TutorialWidget = CreateWidget<UCTutorialWidget>(GetOwningPlayer(), TutorialWidgetClass);
	TutorialWidget->TutorialText->SetText(FText::FromString(TutorialMessage));
	
	for (auto Key : KeysToDisplay)
	{
		if (Key == "LMB" || Key == "RMB" || Key == "Mouse")
		{
			UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			UTexture2D* TextureToSet = Key == "LMB" ? LMBButton : RMBButton;
			TextureToSet = Key == "Mouse" ? Mouse : TextureToSet;

			FSlateBrush SlateBrush;
			SlateBrush.SetImageSize(FVector2D(60.f, 60.f));
			SlateBrush.SetResourceObject(TextureToSet);
			Image->SetBrush(SlateBrush);

			TutorialWidget->KeysHorizontalBox->AddChildToHorizontalBox(Image);
			continue;
		}

		FString Text = TutorialWidget->KeysText->GetText().ToString() + Key;
		TutorialWidget->KeysText->SetText(FText::FromString(Text));
	}

	TutorialVerticalBox->AddChildToVerticalBox(TutorialWidget);
}
