// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Actions/CAction.h"
#include "System/TweenSubsystem.h"
#include "UI/CSpecialAbilityContainerWidget.h"
#include "CAction_UtilitySelectorAction.generated.h"

class ACCommonCharacter;
class UInputAction;
/**
 * 
 */
UCLASS()
class CMP302_API UCAction_UtilitySelectorAction : public UCAction
{
	GENERATED_BODY()

public:

	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;
	
	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

	virtual void StartAction_Implementation(AActor* InInstigator) override;

	void RegisterNewAction(UCAction* ActionToRegister);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ActivateInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	float SpecialAbilityCooldown;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSubclassOf<UCSpecialAbilityContainerWidget> SpecialAbilityContainerWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UMaterialParameterCollection* UIMaterialParameters;
	
	UFUNCTION()
	void ActivateSpecialAbility();
	
private:

	UFUNCTION()
	void CooldownTween(float Value);

	FTweenHandle CooldownTweenHandle;

	UPROPERTY()
	int32 InputBindingHandle;

	int32 CurrentIndex = 0;

	UPROPERTY()
	ACCommonCharacter* Character;

	UPROPERTY()
	TArray<FGameplayTag> SpecialAbilitiesTags;

	UPROPERTY()
	UCSpecialAbilityContainerWidget* SpecialAbilityContainerInstance;
};
