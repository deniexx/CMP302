// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_Sword.generated.h"

class ACCommonCharacter;
/**
 * 
 */
UCLASS()
class CMP302_API UCAction_Sword : public UCAction
{
	GENERATED_BODY()

public:
	
	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;

	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

	virtual void StartAction_Implementation(AActor* InInstigator) override;

protected:

	/** The mesh of the sword */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UStaticMesh* SwordMesh;

	/** The transform to attach the sword to the player */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FTransform AttachTransform;

	/** The slash attack montages (these should be combos, with 3 attacks) */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TArray<UAnimMontage*> SlashAttackMontages;

	/** The animation montage to use when the character reflects a projectile */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* ReflectAnimMontage;

	/** The maximum distance to snap to a target */
	UPROPERTY(EditDefaultsOnly, Category = "Snap")
	float MaxSnapDistance;

	/** The minimum distance to snap to a target */
	UPROPERTY(EditDefaultsOnly, Category = "Snap")
	float MinSnapDistance;

	/** The size of the reflection collision sphere */
	UPROPERTY(EditDefaultsOnly, Category = "Reflect")
	float ReflectSphereSize;

	/** The maximum distance to be able to reflect */
	UPROPERTY(EditDefaultsOnly, Category = "Reflect")
	float MaxReflectDistance;

	/** The size of the collision spheres used for the attack tracing */
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackTraceSphereSize;

	/** The width of the collision spheres used for the attack tracing */
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackTraceWidth;

	/** The length offset of the attack trace sphere */
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackTraceLength;

	/** The maximum amount of attack traces */
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	int32 AttackTraceSphereMaxAmount;

	/** The object types to query for the attacks */
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TArray<TEnumAsByte<EObjectTypeQuery>> AttackObjectTypes;
	
	UPROPERTY()
	UStaticMeshComponent* SwordMeshComponent;

private:

	/** The count of the next combo attack */
	int32 NextComboAttack;
	
	FTimerHandle ResetComboTimer_Handle;

	/** The current montage combo playing */
	UPROPERTY()
	UAnimMontage* CurrentMontage;

	/** Resets the combo when the timer elapses */
	UFUNCTION()
	void ResetComboTimerElapsed(ACCommonCharacter* Character);

	/** Checks if there is terrain in front, we use this to play a different animation if we would hit terrain instead of a enemy */
	bool IsTerrainInFront(ACCommonCharacter* Character) const;

	/** Snaps to target if it is possible (inside the range) */
	bool SnapToTargetIfPossible(ACCommonCharacter* Character) const;

	/** Reflects a projectile if it possible (collides with the reflection sphere) */
	bool ReflectProjectileIfPossible(ACCommonCharacter* Character) const;

	/** Traces for enemy hits */
	bool TraceForEnemyHits(ACCommonCharacter* Character) const;
};
