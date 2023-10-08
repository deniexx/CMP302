// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

class UCSaveGame;
class ACCommonCharacter;
class UCAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActionStateChanged, UCActionComponent*, OwningComponent, UCAction*, Action, bool, bAutoAdded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CMP302_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Attempts to get the action component from the specified actor
	 * @param FromActor The actor to get the action component from
	 * @return If the actor has an action component this function will return that component, otherwise nullptr
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Actions")
	static UCActionComponent* GetActionComponent(AActor* FromActor);
	
	// Sets default values for this component's properties
	UCActionComponent();

	/** The current active gameplay tags on this component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	/**
	 * Adds an action to the ActionComponent
	 * @param TargetActor The target actor/instigator of this add action event
	 * @param ActionClass The action to be added
	 * @param bAutoAdded Whether the action was auto-added, this is mainly used for the action acquired notification
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* TargetActor, TSubclassOf<UCAction> ActionClass, bool bAutoAdded = false);

	/**
	 * Removes an action from the action component
	 * @param ActionToRemove The action to be removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UCAction* ActionToRemove);

	/**
	 * Removes an action from that specified class, this will remove the first instance of that action
	 * @param ActionToRemove The class of the action to be removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveActionByClass(TSubclassOf<UCAction> ActionToRemove);

	/**
	 * Attempts to start an action by a specified tag
	 * @param Instigator The instigator of the action
	 * @param ActionTag The action tag, used to find the action
	 * @return Whether the action was started or not
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByTag(AActor* Instigator, FGameplayTag ActionTag);

	/**
	 * Attempts to stop an action by a specified tag
	 * @param Instigator The instigator of the stop action event
	 * @param ActionTag The action tag, used to find the action
	 * @return Whether the action was stopped (only returns false, if that action is not owned by this component OR the action is not running)
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByTag(AActor* Instigator, FGameplayTag ActionTag);

	/**
	 * Gets an action from a given class, this will only find the first instance of that action
	 * @param ActionClass The class of the action to look for
	 * @return The action of that class, if found, else nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	UCAction* GetAction(TSubclassOf<UCAction> ActionClass);

protected:

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void LoadActions();

	void SaveActions();

	bool IsActionTagTaken(FGameplayTag ActionTag);

	/** The starting actions */
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UCAction>> DefaultActions;

	/** The currently available actions */
	UPROPERTY(BlueprintReadOnly)
	TArray<UCAction*> Actions;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;
	
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionAdded;
	
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionRemoved;
};
