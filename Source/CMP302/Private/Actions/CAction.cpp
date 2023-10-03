// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction.h"

#include "CLogChannels.h"
#include "ActorComponents/CActionComponent.h"
#include "System/CGameplayFunctionLibrary.h"

void UCAction::Initialize(UCActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
}

bool UCAction::IsRunning() const
{
	return bIsRunning;
}

bool UCAction::IsInCooldown() const
{
	return CooldownTimerHandle.IsValid();
}

float UCAction::GetRemainingCooldown()
{
	if (CooldownTimerHandle.IsValid())
	{
		return GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle);
	}

	return 0.f;
}

float UCAction::GetRemainingCooldownFraction()
{
	if (CooldownTimerHandle.IsValid())
	{
		return GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle) / Cooldown;
	}

	return 0.f;
}

void UCAction::OnActionAdded_Implementation(AActor* InInstigator)
{
	UE_LOG(LogCMPActionSystem, Log, TEXT("Granted Action: %s"), *GetNameSafe(this));
	if (bDisplayTutorial)
		UCGameplayFunctionLibrary::AddTutorialMessage(GetOuter(), ActionTutorialText, ActionKeys);
}

void UCAction::OnActionRemoved_Implementation(AActor* InInstigator)
{
	UE_LOG(LogCMPActionSystem, Log, TEXT("Removed Action: %s"), *GetNameSafe(this));
}

void UCAction::TickAction_Implementation(float DeltaTime)
{
}

UWorld* UCAction::GetWorld() const
{
	if (const AActor* Actor = Cast<AActor>(GetOuter()))
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

FString UCAction::GetInCooldownMessage() const
{
	return FString::Printf(TEXT("%ls is not ready."), *ActionName.ToString());
}

UCActionComponent* UCAction::GetOwningComponent() const
{
	return ActionComponent;
}

void UCAction::OnCooldownTimer_Elapsed()
{
	CooldownTimerHandle.Invalidate();
}

bool UCAction::CanStart_Implementation(AActor* InInstigator)
{
	if (IsRunning()) return false;

	if (IsInCooldown())
	{
		UCGameplayFunctionLibrary::AddStatusReportMessage(GetOuter(), GetInCooldownMessage());
		return false;
	}

	const UCActionComponent* Component = GetOwningComponent();

	if (Component->ActiveGameplayTags.HasAnyExact(BlockedTags))
	{
		return false;
	}

	return true;
}

void UCAction::StartAction_Implementation(AActor* InInstigator)
{
	UCActionComponent* Component = GetOwningComponent();
	Component->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
	InstigatorActor = InInstigator;

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
	
	if (bCooldownStartsOnActionStart)
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UCAction::OnCooldownTimer_Elapsed, Cooldown);
}

void UCAction::StopAction_Implementation(AActor* InInstigator)
{
	UCActionComponent* Component = GetOwningComponent();
	Component->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
	InstigatorActor = InInstigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);

	if (!bCooldownStartsOnActionStart)
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UCAction::OnCooldownTimer_Elapsed, Cooldown);
}
