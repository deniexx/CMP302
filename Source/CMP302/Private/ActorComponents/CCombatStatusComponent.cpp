// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CCombatStatusComponent.h"
#include "Character/CCommonCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "System/CGameplayFunctionLibrary.h"
#include "System/TweenSubsystem.h"
#include "CLogChannels.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<int32> CVarImmortal(
	TEXT("Immortal"),
	0,
	TEXT("Makes the player unkillable")
	TEXT(" 0: Can be hit/n")
	TEXT(" 1: Can not be hit/n"),
	ECVF_Cheat
);

UCCombatStatusComponent* UCCombatStatusComponent::GetCombatStatusComponent(const AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCCombatStatusComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

// Sets default values for this component's properties
UCCombatStatusComponent::UCCombatStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bCanBeHit = true;
	bCanChangeStatus = true;
	ColorLerpAlpha = 0.f;
	ColorLerpSpeed = 1.f;
}

void UCCombatStatusComponent::TweenColour(float Value)
{
	const FLinearColor Color = FMath::Lerp(PreviousAttackStatusColor, CurrentAttackStatusColor, Value);
	UKismetMaterialLibrary::SetVectorParameterValue(CharacterOwner, PlayerMaterialParameters,
			TEXT("PlayerAttackStatusColor"), Color);
}

void UCCombatStatusComponent::Init()
{
	CharacterOwner = GetOwner<ACCommonCharacter>();
	if (AttackStatusType == EAttackStatusType::None)
	{
		const APlayerController* Controller = CharacterOwner ? CharacterOwner->GetController<APlayerController>() : nullptr;
		const bool bIsPlayerControlled = Controller != nullptr;
		
		if (!bIsPlayerControlled)
		{
			// Get a random index from the range to use in the enum as red, green or blue
			int index = FMath::RandRange(0, 2) + 1;
			UpdateAttackStatusType(EAttackStatusType(index));
		}
		else
		{
			UpdateAttackStatusType(EAttackStatusType::Red);
		}
	}
}

bool UCCombatStatusComponent::CheckCanBeHit(const FAttackData& AttackData) const
{
	/* Only player is immortal */
	const bool bImmortal = CVarImmortal.GetValueOnAnyThread() > 0 && AttackData.AttackStatusType == EAttackStatusType::Enemy;
	
	if (bImmortal || !bCanBeHit)
	{
		// We can't be hit as we are currently in GodMode or we can't be hit (we might be dashing, etc...)
		return false;
	}

	const APlayerController* Controller = CharacterOwner ? CharacterOwner->GetController<APlayerController>() : nullptr;
	const bool bIsPlayerControlled = Controller != nullptr;
	const bool bAlwaysHit = AttackData.AttackStatusType == EAttackStatusType::White;
	
	if (!bIsPlayerControlled)
	{
		if (AttackData.AttackStatusType != AttackStatusType && !bAlwaysHit)
		{
			// We can't be hit as the attack status types are not the same
			return false;
		}
	}
	else
	{
		if (AttackData.AttackStatusType != EAttackStatusType::Enemy)
		{
			// We can't be hit as this is not an enemy attack
			return false;
		}
	}

	// If we have reached this point, it means the other checks have failed and we can be hit
	return true;
}

void UCCombatStatusComponent::UpdateAttackStatusType(EAttackStatusType NewAttackStatusType)
{
	if (!bCanChangeStatus) return;
	
	PreviousAttackStatus = AttackStatusType;
	AttackStatusType = NewAttackStatusType;

	OnAttackStatusTypeUpdated.Broadcast(PreviousAttackStatus, AttackStatusType);

	ColorLerpAlpha = 0.f;
	PreviousAttackStatusColor =  UKismetMaterialLibrary::GetVectorParameterValue(CharacterOwner, PlayerMaterialParameters, TEXT("PlayerAttackStatusColor"));
	CurrentAttackStatusColor = UCGameplayFunctionLibrary::GetColorFromAttackStatus(AttackStatusType);

	if (TweenHandle.IsValid()) return;
	
	UTweenSubsystem* TweenWorldSubsystem = CharacterOwner->GetGameInstance()->GetSubsystem<UTweenSubsystem>();
	FTweenDynamicDelegate Delegate;
	Delegate.BindDynamic(this, &ThisClass::TweenColour);
	TweenWorldSubsystem->AddTween(TweenHandle, 0, 1, Delegate, 4);
}

void UCCombatStatusComponent::ForceAttackStatusType(EAttackStatusType NewAttackStatusType)
{
	if (!bCanChangeStatus) return;
	
	UpdateAttackStatusType(NewAttackStatusType);
	bCanChangeStatus = false;
}

void UCCombatStatusComponent::StopForceAttackStatusType(bool bReturnToPrevious)
{
	bCanChangeStatus = true;

	if (bReturnToPrevious)
		UpdateAttackStatusType(PreviousAttackStatus);
}

void UCCombatStatusComponent::SetIgnoreHits(bool bActive)
{
	bCanBeHit = bActive;
}

bool UCCombatStatusComponent::TryRegisterHit(const FAttackData& AttackData)
{
	const bool bCanRegisterHit = CheckCanBeHit(AttackData);

	// If we can't be hit, return false and exit the function
	if (!bCanRegisterHit)
	{
		return false;
	}

	// @TODO: Potentially claim a kill if we implement an interface (can be used to track certain statistics)
	OnHitTaken.Broadcast(AttackData);

	return true;
}

UMaterialParameterCollection* UCCombatStatusComponent::GetPlayerMaterialParameters() const
{
	return PlayerMaterialParameters;
}

EAttackStatusType UCCombatStatusComponent::GetAttackStatusType() const
{
	return AttackStatusType;
}
