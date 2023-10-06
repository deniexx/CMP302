// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_PowerOverload.h"

#include "ActorComponents/CCombatStatusComponent.h"
#include "Character/CCommonCharacter.h"
#include "UI/COverloadActionWidget.h"

void UCAction_PowerOverload::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Cooldown = OverloadDuration + 3.f;
	bHasCharge = true;
	
	if (ensure(OverloadActionWidgetClass))
	{
		Character = Cast<ACCommonCharacter>(InInstigator);
		APlayerController* PlayerController = Character->GetController<APlayerController>();
		OverloadActionWidgetInstance = CreateWidget<UCOverloadActionWidget>(PlayerController, OverloadActionWidgetClass);
		OverloadActionWidgetInstance->AddToViewport();
		OverloadActionWidgetInstance->BindPowerOverloadAction(this);
	}
}

void UCAction_PowerOverload::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);

	if (OverloadActionWidgetInstance)
		OverloadActionWidgetInstance->RemoveFromParent();
}

void UCAction_PowerOverload::TickAction_Implementation(float DeltaTime)
{
	Super::TickAction_Implementation(DeltaTime);
}

bool UCAction_PowerOverload::CanStart_Implementation(AActor* InInstigator)
{
	return Super::CanStart_Implementation(InInstigator) && bHasCharge;
}

void UCAction_PowerOverload::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	UCCombatStatusComponent* CombatStatusComponent = UCCombatStatusComponent::GetCombatStatusComponent(InInstigator);
	CombatStatusComponent->ForceAttackStatusTypeForDuration(EAttackStatusType::White, OverloadDuration);
	CombatStatusComponent->IgnoreHitsForDuration(OverloadDuration);
}

void UCAction_PowerOverload::AddCharge()
{
	bHasCharge = true;

	/*
	if (OverloadActionWidgetInstance)
		OverloadActionWidgetInstance->AddToViewport();
	else
	{
		APlayerController* PlayerController = Character->GetController<APlayerController>();
		OverloadActionWidgetInstance = CreateWidget<UCOverloadActionWidget>(PlayerController, OverloadActionWidgetClass);
		OverloadActionWidgetInstance->AddToViewport();
		OverloadActionWidgetInstance->BindPowerOverloadAction(this);
	}*/
}
