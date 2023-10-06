// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGameplayFunctionLibrary.generated.h"

class ACCommonCharacter;
class ACProjectile;
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
	 * 
	 * @param Color The color used to find the attack status from
	 * @return An attack status based on the color, will never return Enemy, will return None if given an Invalid Color
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayFunctionLibrary")
	static EAttackStatusType GetAttackStatusFromColor(FLinearColor Color);
	
	/**
	 * Performs an attempt to register a hit on the target actor
	 * @param AttackData AttackData to be used for hit registration
	 * @param TargetActor The target of the hit
	 * @return Whether the hit was successful or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayFunctionLibrary")
	static bool TryRegisterHit(const FAttackData& AttackData, AActor* TargetActor);

	/**
	 * Spawns a projectile with a given class as a template
	 * @param WorldContextObject Object used to get the world
	 * @param ProjectileClass The class used as a template to spawn the projectile as
	 * @param Character The Character owner of the projectile, will be used to determine things like heading and rotation
	 * @return The spawned projectile, will return nullptr, if Character or ProjectileClass are not valid
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static ACProjectile* SpawnProjectile(const UObject* WorldContextObject, TSubclassOf<ACProjectile> ProjectileClass, ACCommonCharacter* Character);

	/**
	 * Gets the status report subsystem
	 * @param WorldContextObject Object used to get the world
	 * @return The status report subsystem, if not one has not been found will return nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static UCStatusReportSubsystem* GetStatusReportSubsystem(const UObject* WorldContextObject);

	/**
	 * Displays a message to the status report widget
	 * @param WorldContextObject Object used to get the world
	 * @param Message The message to be posted in the status report widget
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void AddStatusReportMessage(const UObject* WorldContextObject, const FString& Message);

	/**
	 * Displays a tutorial message on the screen
	 * @param WorldContextObject Object used to get the world
	 * @param TutorialMessage The tutorial message to be displayed
	 * @param KeysToDisplay The keys to display, that used to instigate the action
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
	static void AddTutorialMessage(const UObject* WorldContextObject, const FString& TutorialMessage, const TArray<FString>& KeysToDisplay);
};
