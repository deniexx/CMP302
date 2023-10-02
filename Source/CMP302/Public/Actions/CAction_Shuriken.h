// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_Shuriken.generated.h"

class ACProjectile;
/**
 * 
 */
UCLASS()
class CMP302_API UCAction_Shuriken : public UCAction
{
	GENERATED_BODY()

public:
	
	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;

	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

	virtual void TickAction_Implementation(float DeltaTime) override;

	virtual void StartAction_Implementation(AActor* InInstigator) override;

protected:

	virtual FString GetInCooldownMessage() const override;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ACProjectile> ShurikenProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UStaticMesh* ShurikenMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FTransform AttachTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* ShurikenThrowMontage;
	
	UPROPERTY(EditAnywhere, Category = "Visuals")
	TSubclassOf<UUserWidget> ShurikenCooldownWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Visuals")
	UMaterialParameterCollection* UIParameterCollection;

	UPROPERTY()
	UUserWidget* ShurikenCooldownWidget;

	UPROPERTY()
	UStaticMeshComponent* ShurikenMeshComponent;

	UFUNCTION()
	void AttackDelayTimer_Elapsed(ACCommonCharacter* Character);
};
