// Coded by: Denis Dimitrov for CMP302


#include "UI/COverloadActionWidget.h"

void UCOverloadActionWidget::BindPowerOverloadAction(UCAction_PowerOverload* InPowerOverloadAction)
{
	PowerOverloadAction = InPowerOverloadAction;
}

void UCOverloadActionWidget::UpdateChargeReceived_Implementation()
{
}
