// Coded by: Denis Dimitrov for CMP302


#include "UI/CDashUserWidget.h"

#include "Actions/CAction_Dash.h"
#include "Kismet/KismetMaterialLibrary.h"


void UCDashUserWidget::BindDashAction(UCAction_Dash* InDashAction)
{
	DashAction = InDashAction;
}

void UCDashUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DashAction)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, UIParameters, "DashTimerProgress", DashAction->GetDashTimerProgress());
	}
}
