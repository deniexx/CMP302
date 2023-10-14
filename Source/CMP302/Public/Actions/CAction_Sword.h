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

	virtual void StopAction_Implementation(AActor* InInstigator) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UStaticMesh* SwordMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FTransform AttachTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TArray<UAnimMontage*> SlashAttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* ReflectAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Snap")
	float MaxSnapDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Snap")
	float MinSnapDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Reflect")
	float ReflectSphereSize;

	UPROPERTY(EditDefaultsOnly, Category = "Reflect")
	float MaxReflectDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackTraceSphereSize;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackTraceWidth;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackTraceLength;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	int32 AttackTraceSphereMaxAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TArray<TEnumAsByte<EObjectTypeQuery>> AttackObjectTypes;

	UPROPERTY()
	UStaticMeshComponent* SwordMeshComponent;

private:

	bool IsTerrainInFront(ACCommonCharacter* Character) const;
	
	bool SnapToTargetIfPossible(ACCommonCharacter* Character) const;

	bool ReflectProjectileIfPossible(ACCommonCharacter* Character) const;

	bool TraceForEnemyHits(ACCommonCharacter* Character) const;
};
