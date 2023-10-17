// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_UtilitySelectorAction.h"

#include "CLogChannels.h"
#include "ActorComponents/CActionComponent.h"
#include "Character/CCommonCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"

void UCAction_UtilitySelectorAction::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Character = Cast<ACCommonCharacter>(InInstigator);
	APlayerController* PlayerController = Character->GetController<APlayerController>();
	
	SpecialAbilityContainerInstance = CreateWidget<UCSpecialAbilityContainerWidget>(PlayerController, SpecialAbilityContainerWidgetClass);
	SpecialAbilityContainerInstance->AddToViewport();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		InputBindingHandle = EnhancedInputComponent->BindAction(ActivateInputAction, ETriggerEvent::Started, this, &ThisClass::ActivateSpecialAbility).GetHandle();
	}
}

void UCAction_UtilitySelectorAction::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);
	
	SpecialAbilityContainerInstance->RemoveFromParent();

	const APlayerController* PlayerController = Character->GetController<APlayerController>();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		EnhancedInputComponent->RemoveActionBinding(InputBindingHandle);
	}
}

void UCAction_UtilitySelectorAction::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	const int32 PreviousIndex = CurrentIndex;
	if (++CurrentIndex >= SpecialAbilitiesTags.Num())
		CurrentIndex = 0;
	
	SpecialAbilityContainerInstance->UpdateActiveAction(CurrentIndex, PreviousIndex);
	StopAction(InInstigator);
}

void UCAction_UtilitySelectorAction::RegisterNewAction(UCAction* ActionToRegister)
{
	SpecialAbilitiesTags.Add(ActionToRegister->ActionTag);
	
	SpecialAbilityContainerInstance->AddNewAbilityWidget(ActionToRegister);
}

void UCAction_UtilitySelectorAction::ActivateSpecialAbility()
{
	if (CooldownTweenHandle.IsValid() || SpecialAbilitiesTags.Num() <= 0)
		return;

	UTweenSubsystem* TweenSubsystem = Character->GetGameInstance()->GetSubsystem<UTweenSubsystem>();
	FTweenDynamicDelegate Delegate;
	Delegate.BindDynamic(this, &ThisClass::CooldownTween);
	TweenSubsystem->AddTween(CooldownTweenHandle, 0.f, SpecialAbilityCooldown, Delegate, 1);
	
	GetOwningComponent()->StartActionByTag(Character, SpecialAbilitiesTags[CurrentIndex]);
}

void UCAction_UtilitySelectorAction::CooldownTween(float Value)
{
	const float Progress = Value / SpecialAbilityCooldown;
	UE_LOG(LogCMPActionSystem, Warning, TEXT("%f Progress"), Value);
	UKismetMaterialLibrary::SetScalarParameterValue(GetOuter(), UIMaterialParameters,"SpecialAbilityCooldownProgress", Progress);
}
