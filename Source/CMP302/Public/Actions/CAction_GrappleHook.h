// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_GrappleHook.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UCWorldUserWidget;
class ACPlayerCharacter;
/**
 * 
 */
UCLASS()
class CMP302_API UCAction_GrappleHook : public UCAction
{
	GENERATED_BODY()
	
public:

	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;

	virtual void TickAction_Implementation(float DeltaTime) override;

	virtual void StartAction_Implementation(AActor* InInstigator) override;

	virtual void StopAction_Implementation(AActor* InInstigator) override;

	virtual bool CanStart_Implementation(AActor* InInstigator) override;

protected:

	/** The object types to query */
	UPROPERTY(EditDefaultsOnly, Category = "Grappling")
	TArray<TEnumAsByte<EObjectTypeQuery>> GrappleHookObjectTypes;

	/** The max range for grappling */
	UPROPERTY(EditDefaultsOnly, Category = "Grappling")
	float MaxGrappleRange;

	/** The force to be applied when grappling towards the target */
	UPROPERTY(EditDefaultsOnly, Category = "Grappling")
	float GrapplePower;

	/** The world widget to be displayed on the target we can grapple to */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSubclassOf<UCWorldUserWidget> WorldUserWidgetClass;

	/** The montage to be used for the grappling */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* GrappleMontage;

	/** The effect to play when grappling */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UNiagaraSystem* GrappleEffect;

private:
	/**
	 * Finds the best target to grapple to
	 * @param HitResults The hit results from the collision check
	 */
	void FindBestTarget(const TArray<FHitResult>& HitResults);

	/** The current grapple target */
	UPROPERTY()
	AActor* GrappleTarget;
	
	UPROPERTY()
	ACPlayerCharacter* Character;
	
	UPROPERTY()
	UCWorldUserWidget* WorldUserWidgetInstance;

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent;
};
