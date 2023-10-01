// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "Components/ActorComponent.h"
#include "CCombatStatusComponent.generated.h"

class ACCommonCharacter;
class ACProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusHitTaken, const FAttackData&, AttackData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackStatusUpdated, EAttackStatusType, PreviousType, EAttackStatusType, NewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CMP302_API UCCombatStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
	static UCCombatStatusComponent* GetCombatStatusComponent(const AActor* FromActor);
	
	// Sets default values for this component's properties
	UCCombatStatusComponent();

	/** Event to be bound to when we get hit */
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnStatusHitTaken OnHitTaken;

	/** Event to be bound to when we swap the attack status, for example changing the material on the player */
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttackStatusUpdated OnAttackStatusTypeUpdated;

	void Init();

protected:
	
	/** Whether the player can be hit, this will be enabled during iFrames(invincibility frames) */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bCanBeHit;

	/** Current attack status type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	EAttackStatusType AttackStatusType = EAttackStatusType::None;
	
	/** This will drive player material parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UMaterialParameterCollection* PlayerMaterialParameters;

private:

	/**
	 * Checks if the player can be hit
	 * @param AttackData Attack data to be used to perform the check
	 * @return Whether the character can be hit or not
	 */
	bool CheckCanBeHit(const FAttackData& AttackData) const;

	/** The previous color of the attack status */
	UPROPERTY()
	FLinearColor PreviousAttackStatusColor;

	/** The current color of the attack status */
	UPROPERTY()
	FLinearColor CurrentAttackStatusColor;

	UPROPERTY()
	ACCommonCharacter* CharacterOwner;

	/** The Lerp Progress */
	float ColorLerpAlpha;

	/** How fast should the color lerp */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ColorLerpSpeed;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Updates the attack status type and broadcasts a delegate
	 * @param NewAttackStatusType The new attack status type to be applied
	 */
	void UpdateAttackStatusType(EAttackStatusType NewAttackStatusType);

	/**
	 * Gets the attack status of the character
	 * @return The current attack status of the character
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	EAttackStatusType GetAttackStatusType() const;

	/**
	 * Attempt to register a hit with the actor owning the component
	 * @param AttackData Attack data to be used to determine whether the hit should be registered
	 * @returns Whether the hit was successfully registered
	 */
	bool TryRegisterHit(const FAttackData& AttackData) const;

	/**
	 * Gets the player material parameter collection
	 * @return The player material parameter collection
	 */
	UMaterialParameterCollection* GetPlayerMaterialParameters() const;
};
