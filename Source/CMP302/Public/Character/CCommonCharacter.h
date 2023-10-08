// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../CCustomTypes.h"
#include "CCommonCharacter.generated.h"

class UCCombatStatusComponent;
class UCActionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLanded, const FHitResult&, Hit);

UCLASS()
class CMP302_API ACCommonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACCommonCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintAssignable)
	FOnCharacterLanded OnCharacterLanded;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Called when a hit has been taken by this character
	 * @param AttackData The attack data for the hit taken
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnHitTaken_BP(const FAttackData& AttackData);
	
	/** Combat Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UCCombatStatusComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCActionComponent* ActionComponent;

	/** This function is bound to the hit taken delegate on the combat component */
	UFUNCTION()
    virtual void OnHitTaken(const FAttackData& AttackData);

	/** An array of the mesh dynamic materials, used for the appearance */
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MeshDynamicMaterials;

	/** Whether we are currently dead */
	bool bDead;

	/** The appearance alpha value */
	float AppearanceAlpha;

	virtual void TweenAppearance(float Value);

	void StartTweenAppearance();

public:

	virtual void Landed(const FHitResult& Hit) override;

	/**
	 * Sets up the actor to be ready, this can include reviving its
	 */
	virtual void ReadyActor();

	/**
	 * Check if the actor is dead
	 * @return Whether the actor is dead
	 */
	bool IsDead() const;
};
