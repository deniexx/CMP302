// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGameplayFunctionLibrary.generated.h"

class UCStatusReportSubsystem;
/**
 * 
 */
UCLASS()
class CMP302_API UCGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	
	/**
	 * Gives a color based on the provided attack status
	 * @param AttackStatus Attack Status to grab the color from
	 * @return A color based on the AttackStatus, will return Black if we have been given an Invalid Attack Status
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayFunctionLibrary")
	static FLinearColor GetColorFromAttackStatus(EAttackStatusType AttackStatus);
	
	/**
	 * Performs an attempt to register a hit on the target actor
	 * @param AttackData AttackData to be used for hit registration
	 * @param TargetActor The target of the hit
	 * @return Whether the hit was successful or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayFunctionLibrary")
	static bool TryRegisterHit(const FAttackData& AttackData, AActor* TargetActor);


	UFUNCTION(BlueprintCallable, Category = "GameplayFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static ACProjectile* SpawnProjectile(const UObject* WorldContextObject, TSubclassOf<ACProjectile> ProjectileClass, ACCommonCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "GameplayFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UCStatusReportSubsystem* GetStatusReportSubsystem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "GameplayFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void AddStatusReportMessage(const UObject* WorldContextObject, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "GameplayFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void UpdateStatusImage(const UObject* WorldContextObject, UTexture2D* StatusTexture);
};
