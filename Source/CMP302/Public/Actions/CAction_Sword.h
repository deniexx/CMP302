// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_Sword.generated.h"

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

	UPROPERTY()
	UStaticMeshComponent* SwordMeshComponent;

private:

	UFUNCTION()
	void OnSwordHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
