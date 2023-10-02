// Coded by: Denis Dimitrov for CMP302


#include "UI/CStatusReportWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
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

void UCStatusReportWidget::UpdateStatusImage(UTexture2D* StatusTexture) const
{
	StatusImage->SetBrushFromTexture(StatusTexture);
}
