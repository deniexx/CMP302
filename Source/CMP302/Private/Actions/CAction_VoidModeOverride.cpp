// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_VoidModeOverride.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "ActorComponents/CActionComponent.h"
#include "Actors/CVoidActor.h"
#include "Character/CCommonCharacter.h"

void UCAction_VoidModeOverride::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Character = Cast<ACCommonCharacter>(InInstigator);
	const APlayerController* PlayerController = Character ? Character->GetController<APlayerController>() : nullptr;
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		InputBindingHandle = EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::ActivateActionOnPress).GetHandle();
	}
	
	for (TActorIterator<ACVoidActor> It(GetWorld()); It; ++It)
	{
		if (ACVoidActor* VoidActor = *It)
		{
			VoidActors.AddUnique(VoidActor);
		}
	}
}

void UCAction_VoidModeOverride::ActivateActionOnPress()
{
	GetOwningComponent()->StartActionByTag(Character, ActionTag);
}

void UCAction_VoidModeOverride::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);

	VoidActors.Empty();
	
	const APlayerController* PlayerController = Character ? Character->GetController<APlayerController>() : nullptr;
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		EnhancedInputComponent->RemoveActionBinding(InputBindingHandle);
	}
}

void UCAction_VoidModeOverride::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	for (ACVoidActor* VoidActor : VoidActors)
	{
		if (VoidActor)
			VoidActor->SwitchVoidState();
	}

	StopAction(InInstigator);
}
