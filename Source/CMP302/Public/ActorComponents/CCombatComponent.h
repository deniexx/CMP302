// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "Components/ActorComponent.h"
#include "CCombatComponent.generated.h"

class ACProjectile;
class ACCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitTaken, const FAttackData&, AttackData);
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
	 * @param InShurikenMesh The shuriken mesh of the character
	 */
	void Init(UStaticMeshComponent* InShurikenMesh);

	/**
	 * This makes it so that we can use the slash attack again, meaning our previous animation has ended
	 */
	void ResetSlashAttack();

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
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
	float FireProjectileDelay;

	/** This is the delay between slash attacks */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float SlashAttackAnimDelay;

	/** This is the fire projectile montage */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* FireProjectileMontage;

	/** A timer used to reset and allow you fire the projectile again */
	UPROPERTY()
	FTimerHandle FireProjectileTimerHandle;

	/** This is used to prevent spamming the slash attack */
	UPROPERTY()
	FTimerHandle SlashAttackTimerHandle;

private:
	/**
	 * Checks if the player can be hit
	 * @param AttackData Attack data to be used to perform the check
	 * @return Whether the character can be hit or not
	 */
	bool CheckCanBeHit(const FAttackData& AttackData) const;

	/**
	 * This function is called after the fire projectile timer finishes
	 */
	UFUNCTION()
	void FireProjectile_TimerElapsed();

	/**
	 * This does the actual reset of the slash attack to be able to be used again
	 */
	UFUNCTION()
	void SlashAttack_TimerElapsed();

	/** The previous color of the attack status */
	UPROPERTY()
	FLinearColor PreviousAttackStatusColor;

	/** The current color of the attack status */
	UPROPERTY()
	FLinearColor CurrentAttackStatusColor;

	/** The Character owning this component */
	UPROPERTY()
	ACCharacter* CharacterOwner;

	/** Used to cache the shuriken mesh, for ease of use */
	UPROPERTY()
	UStaticMeshComponent* ShurikenMesh;

	/** The Lerp Progress */
	float ColorLerpAlpha;

	/** Can we use the slash attack right now */
	bool bIsSlashAttackReady;

	/** Can we fire a projectile right now */
	bool bCanFireProjectile;

	/** How fast should the color lerp */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float ColorLerpSpeed;

	/** An array with AnimMontages to pick for the slash attack */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<UAnimMontage*> SlashAttackMontages;

public:
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
	 * Spawns the actual projectile, this is called from an AnimNotify
	 */
	void SpawnProjectile();

	/**
	 * Fires a projectile towards the crosshair
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireProjectile();

	/**
	 * Uses the slash attack as long as we not dashing or attack currently
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SlashAttack();

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
