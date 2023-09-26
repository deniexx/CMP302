// Coded by: Denis Dimitrov for CMP302


#include "System/CGameplayFunctionLibrary.h"

#include "ActorComponents/CCombatComponent.h"

FColor UCGameplayFunctionLibrary::GetColorFromAttackStatus(EAttackStatusType AttackStatus)
{
	FColor ToReturn;
	
	switch (AttackStatus)
	{
		case EAttackStatusType::Green:
			ToReturn = FColor::Green;
			break;
		case EAttackStatusType::Red:
			ToReturn = FColor::Red;
			break;
		case EAttackStatusType::Blue:
			ToReturn = FColor::Blue;
			break;
		case EAttackStatusType::Enemy:
			ToReturn = FColor::Red;
			break;
		case EAttackStatusType::White:
			ToReturn = FColor::White;
			break;
		default:
			ToReturn = FColor::Black;
			break;;
	}

	return ToReturn;
}

bool UCGameplayFunctionLibrary::TryRegisterHit(const FAttackData& AttackData, AActor* TargetActor)
{
	UCCombatComponent* CombatComponent = TargetActor ? UCCombatComponent::GetCombatComponent(TargetActor) : nullptr;
	if (CombatComponent)
	{
		return CombatComponent->TryRegisterHit(AttackData);
	}

	return false;
}
