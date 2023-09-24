// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "GameFramework/CheatManager.h"
#include "CCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class CMP302_API UCCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	/**
	 * Hit the current target under your crosshair
	 */
	UFUNCTION(Exec)
	void HitTarget(EAttackStatusType AttackStatus = EAttackStatusType::White);
};
