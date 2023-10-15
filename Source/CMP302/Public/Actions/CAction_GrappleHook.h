// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_GrappleHook.generated.h"

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

	virtual bool CanStart_Implementation(AActor* InInstigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Grappling")
	TArray<TEnumAsByte<EObjectTypeQuery>> GrappleHookObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Grappling")
	float MaxGrappleRange;

	UPROPERTY(EditDefaultsOnly, Category = "Grappling")
	float GrapplePower;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSubclassOf<UCWorldUserWidget> WorldUserWidgetClass;

private:

	void FindBestTarget(const TArray<FHitResult>& HitResults);

	UPROPERTY()
	AActor* GrappleTarget;
	
	UPROPERTY()
	ACPlayerCharacter* Character;

	UPROPERTY()
	UCWorldUserWidget* WorldUserWidgetInstance;
};
