// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_SpecialAbility.h"

#include "Actions/CAction_UtilitySelectorAction.h"
#include "ActorComponents/CActionComponent.h"

void UCAction_SpecialAbility::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	UCAction_UtilitySelectorAction* UtilitySelectorAction = Cast<UCAction_UtilitySelectorAction>(
		GetOwningComponent()->GetAction(UCAction_UtilitySelectorAction::StaticClass()));

	if (UtilitySelectorAction)
	{
		UtilitySelectorAction->RegisterNewAction(this);
	}
	else
	{
		GetOwningComponent()->RemoveAction(this);
	}
}
