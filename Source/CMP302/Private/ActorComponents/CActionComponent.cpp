// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CActionComponent.h"

#include "CLogChannels.h"
#include "Actions/CAction.h"

UCActionComponent* UCActionComponent::GetActionComponent(const AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCActionComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

// Sets default values for this component's properties
UCActionComponent::UCActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (const TSubclassOf<UCAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UCActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<UCAction*> ActionsCopy = Actions;
	for (UCAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

bool UCActionComponent::IsActionTagTaken(FGameplayTag ActionTag)
{
	for (const UCAction* Action : Actions)
	{
		if (Action->ActionTag.MatchesTagExact(ActionTag))
			return true;
	}

	return false;
}

void UCActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (UCAction* Action : Actions)
	{
		Action->TickAction(DeltaTime);
	}
}

void UCActionComponent::AddAction(AActor* TargetActor, TSubclassOf<UCAction> ActionClass)
{
	if (!ensure(ActionClass)) return;
	
	UCAction* NewAction = NewObject<UCAction>(GetOwner(), ActionClass);
	
	// Do not allow multiple actions bound to the same tag
	if (IsActionTagTaken(NewAction->ActionTag)) return;
	
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		NewAction->OnActionAdded(GetOwner());
		Actions.Add(NewAction);
		OnActionAdded.Broadcast(this, NewAction);

		if (NewAction->bAutoStart && NewAction->CanStart(TargetActor)) NewAction->StartAction(TargetActor);
	}
}

void UCActionComponent::RemoveAction(UCAction* ActionToRemove)
{
	if (!ActionToRemove) return;
	
	if (ActionToRemove->IsRunning()) ActionToRemove->StopAction(GetOwner());

	OnActionRemoved.Broadcast(this, ActionToRemove);
	Actions.Remove(ActionToRemove);
}

void UCActionComponent::RemoveActionByClass(TSubclassOf<UCAction> ActionToRemove)
{
	UCAction* Action = GetAction(ActionToRemove);
	Action->OnActionRemoved(GetOwner());
	RemoveAction(Action);
}

bool UCActionComponent::StartActionByTag(AActor* Instigator, FGameplayTag ActionTag)
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->ActionTag.MatchesTagExact(ActionTag))
		{
			if (!Action->CanStart(Instigator))
			{
				UE_LOG(LogCMPActionSystem, Log, TEXT("Failed to start: %s, can start returned FALSE"), *ActionTag.ToString());
				continue;
			}

			UE_LOG(LogCMPActionSystem, Log, TEXT("Starting: %s"), *ActionTag.ToString());
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UCActionComponent::StopActionByTag(AActor* Instigator, FGameplayTag ActionTag)
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->ActionTag.MatchesTagExact(ActionTag))
		{
			if (Action->IsRunning())
			{
				UE_LOG(LogCMPActionSystem, Log, TEXT("Stopping: %s"), *ActionTag.ToString());
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	UE_LOG(LogCMPActionSystem, Log, TEXT("Attempted to Stop Action with tag: %s"), *ActionTag.ToString());
	return false;
}

UCAction* UCActionComponent::GetAction(TSubclassOf<UCAction> ActionClass)
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}
