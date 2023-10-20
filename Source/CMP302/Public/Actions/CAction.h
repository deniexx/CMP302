// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CAction.generated.h"

class UCActionComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class CMP302_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Initializes the action with the given action component
	 * @param NewActionComponent The Action Component to be assigned to this action
	 */
	void Initialize(UCActionComponent* NewActionComponent);

	/** Whether the action should start when it is added */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	/**
	 * Check whether the action can be started in the current state
	 * @param InInstigator Instigator of the action
	 * @return Whether the action can be started
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InInstigator);

	/**
	 * Checks whether the action is running
	 * @return Whether the action is currently running
	 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	/**
	 * Checks whether the action is in cooldown
	 * @return Whether the action is currently in cooldown
	 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsInCooldown() const;

	/**
	 * Gets the remaining cooldown of the action
	 * @return The remaining cooldown in seconds
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
	float GetRemainingCooldown();

	/**
	 * Gets the remaining cooldown faction of the action
	 * @return The remaining cooldown in a value of 0-1
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
	float GetRemainingCooldownFraction();

	/** Action Tag to be associated with this action */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTag ActionTag;

	/**
	 * An event that is called when the action has been added
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void OnActionAdded(AActor* InInstigator);

	/**
	 * An event that is called when the action has been removed
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void OnActionRemoved(AActor* InInstigator);

	/**
	 * Ticks the action, this is called every frame
	 * @param DeltaTime The deltaTime between the frames
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void TickAction(float DeltaTime);

	/**
	 * Starts the action
	 * @param InInstigator The instigator of the action
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InInstigator);

	/**
	 * Stops the action
	 * @param InInstigator The instigator of the stop of the action
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* InInstigator);

	/** Gets the world the action lives in */
	virtual UWorld* GetWorld() const override;

	/** The icon of the action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	/** A name used for the UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FText ActionName;

	/** A description used for the UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (MultiLine = 3))
	FText ActionDescription;

	/** The cost of the action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop")
	int32 Cost;
	
protected:

	/** Gets the cooldown message for the report status subsystem for the action */
	virtual FString GetInCooldownMessage() const;

	/** Cached Action Component */
	UPROPERTY()
	UCActionComponent* ActionComponent;

	/** Tags to be granted to the owning component by this action */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/** Tags that block the execution of this action */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	/** The cooldown of this action in seconds */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	float Cooldown;

	/** Should the cooldown start when this action is started,
	 * if this is false, the cooldown will start, when the action is stopped */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bCooldownStartsOnActionStart;

	/**
	 * Gets the owning component of the action
	 * @return The owning component of this action
	 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	UCActionComponent* GetOwningComponent() const;

	/** Whether the action is currently running */
	UPROPERTY()
	bool bIsRunning;

	/** The instigator of this action */
	UPROPERTY(BlueprintReadOnly)
	AActor* InstigatorActor;
	
	/** The keys used for this action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TArray<FString> ActionKeys;

	/** A tutorial text to display */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FString ActionTutorialText;
	
	/** Handle of the cooldown timer */
	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	/** The event to be called when the action cooldown timer has elapsed */
	UFUNCTION()
	void OnCooldownTimer_Elapsed();
	
};
