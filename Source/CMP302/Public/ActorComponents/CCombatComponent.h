// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "Components/ActorComponent.h"
#include "CCombatComponent.generated.h"

class ACProjectile;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackStatusTypeUpdated, EAttackStatusType, PreviousType, EAttackStatusType, NewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CMP302_API UCCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	 * Gets the combat component from the specified actor
	 * @param FromActor The Actor to grab the Component from
	 * @returns The combat component or nullptr if the actor does not have one
	 */
	static UCCombatComponent* GetCombatComponent(const AActor* FromActor);
	
	UCCombatComponent();

	/** Event to be bound to when we get hit */
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnHitTaken OnHitTaken;

	/** Event to be bound to when we swap the attack status, for example changing the material on the player */
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttackStatusTypeUpdated OnAttackStatusTypeUpdated;

	/**
	 * Initializes the enemies with a Random Attack Status if one has not been set
	 */
	void Init();

	/**
	 * Fires a projectile towards the crosshair
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireProjectile();

protected:
	
	/** Whether the player can be hit, this will be enabled during iFrames(invincibility frames) */
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bCanBeHit;

	/** Current attack status type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	EAttackStatusType AttackStatusType = EAttackStatusType::None;

	/** The class to be used to spawn projectile with */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<ACProjectile> ProjectileClass;

	/** This will drive player material parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UMaterialParameterCollection* PlayerMaterialParameters;

	/** This is the delay between the animation and the actual firing of the projectile */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireProjectileAnimDelay;

	/** This is the fire projectile montage */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* FireProjectileMontage;
	
	UPROPERTY()
	FTimerHandle FireProjectileTimerHandle;

private:
	/**
	 * Checks if the player can be hit
	 * @param AttackData Attack data to be used to perform the check
	 * @return Whether the character can be hit or not
	 */
	bool CheckCanBeHit(const FAttackData& AttackData) const;

	/**
	 * Performs the actual firing of the projectile
	 */
	UFUNCTION()
	void FireProjectile_TimerElapsed();

public:
	/**
	 * Updates the attack status type and broadcasts a delegate
	 * @param NewAttackStatusType The new attack status type to be applied
	 */
	void UpdateAttackStatusType(EAttackStatusType NewAttackStatusType);

	/**
	 * Attempt to register a hit with the actor owning the component
	 * @param AttackData Attack data to be used to determine whether the hit should be registered
	 * @returns Whether the hit was successfully registered
	 */
	bool TryRegisterHit(const FAttackData& AttackData) const;
};
