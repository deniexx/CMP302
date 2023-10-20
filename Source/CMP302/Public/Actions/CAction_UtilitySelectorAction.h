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

	/** The input action used to active the special ability */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ActivateInputAction;

	/** The special ability cooldown to be applied when it has been used */
	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	float SpecialAbilityCooldown;

	/** The widget container for the special abilities */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSubclassOf<UCSpecialAbilityContainerWidget> SpecialAbilityContainerWidgetClass;

	/** The UIParameter collection */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UMaterialParameterCollection* UIMaterialParameters;

	/** Activates a special ability, this is bound to the ActivateInputAction */
	UFUNCTION()
	void ActivateSpecialAbility();
	
private:

	/** A tween that is used to manage the cooldown of the special abilities */
	UFUNCTION()
	void CooldownTween(float Value);

	FTweenHandle CooldownTweenHandle;

	/** The input binding handle for the ActivateInputAction, this is used to unbind it */
	UPROPERTY()
	int32 InputBindingHandle;

	/** The current index of the selected action */
	int32 CurrentIndex = 0;

	UPROPERTY()
	ACCommonCharacter* Character;

	UPROPERTY()
	TArray<FGameplayTag> SpecialAbilitiesTags;

	UPROPERTY()
	UCSpecialAbilityContainerWidget* SpecialAbilityContainerInstance;
};
