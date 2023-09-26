// Coded by: Denis Dimitrov for CMP302


#include "System/CGameplayFunctionLibrary.h"

#include "ActorComponents/CCombatComponent.h"

FLinearColor UCGameplayFunctionLibrary::GetColorFromAttackStatus(EAttackStatusType AttackStatus)
{
	FLinearColor ToReturn;
	
	switch (AttackStatus)
	{
		case EAttackStatusType::Green:
			ToReturn = FLinearColor::Green;
			break;
		case EAttackStatusType::Red:
			ToReturn = FLinearColor::Red;
			break;
		case EAttackStatusType::Blue:
			ToReturn = FLinearColor::Blue;
			break;
		case EAttackStatusType::Enemy:
			ToReturn = FLinearColor::Red;
			break;
		case EAttackStatusType::White:
			ToReturn = FLinearColor::White;
			break;
		default:
			ToReturn = FLinearColor::Black;
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
