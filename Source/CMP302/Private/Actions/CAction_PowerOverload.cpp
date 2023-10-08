// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_PowerOverload.h"

#include "ActorComponents/CCombatStatusComponent.h"
#include "Character/CCommonCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "System/CGameplayFunctionLibrary.h"
#include "UI/COverloadActionWidget.h"

void UCAction_PowerOverload::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Cooldown = OverloadDuration + 3.f;
	
	if (ensure(OverloadActionWidgetClass))
	{
		Character = Cast<ACCommonCharacter>(InInstigator);
		APlayerController* PlayerController = Character->GetController<APlayerController>();
		OverloadActionWidgetInstance = CreateWidget<UCOverloadActionWidget>(PlayerController, OverloadActionWidgetClass);
		OverloadActionWidgetInstance->AddToViewport();
		OverloadActionWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
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

	if (IsRunning())
	{
		TimeElapsed += DeltaTime;
		UKismetMaterialLibrary::SetScalarParameterValue(GetOuter(), UIMaterialParameters, TEXT("OverloadTimerProgress"), 1 - (TimeElapsed / OverloadDuration));
		
		if (TimeElapsed >= OverloadDuration)
		{
			StopAction(Character);
		}
	}
}

bool UCAction_PowerOverload::CanStart_Implementation(AActor* InInstigator)
{
	const bool bCanStart = Super::CanStart_Implementation(InInstigator);

	if (!bCanStart)
		return false;
	
	if (!bHasCharge)
	{
		UCGameplayFunctionLibrary::AddStatusReportMessage(Character, TEXT("No battery found for overcharge"));
	}

	return true;
}

void UCAction_PowerOverload::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	UCCombatStatusComponent* CombatStatusComponent = UCCombatStatusComponent::GetCombatStatusComponent(InInstigator);
	CombatStatusComponent->ForceAttackStatusType(EAttackStatusType::White);
	CombatStatusComponent->SetIgnoreHits(true);
	OverloadActionWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	bHasCharge = false;
	TimeElapsed = 0.f;
}

void UCAction_PowerOverload::StopAction_Implementation(AActor* InInstigator)
{
	Super::StopAction_Implementation(InInstigator);
	UCCombatStatusComponent* CombatStatusComponent = UCCombatStatusComponent::GetCombatStatusComponent(InInstigator);
	CombatStatusComponent->StopForceAttackStatusType(true);
	CombatStatusComponent->SetIgnoreHits(false);
	OverloadActionWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
}

void UCAction_PowerOverload::AddCharge()
{
	bHasCharge = true;
}
