// Coded by: Denis Dimitrov for CMP302


#include "System/CGameplayFunctionLibrary.h"

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
