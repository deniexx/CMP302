// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CMP302_API UCGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Gives a color based on the provided attack status
	 * @param AttackStatus Attack Status to grab the color from
	 * @return A color based on the AttackStatus, will return Black if we have been given an Invalid Attack Status
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayFunctionLibrary")
	static FColor GetColorFromAttackStatus(EAttackStatusType AttackStatus);
};
