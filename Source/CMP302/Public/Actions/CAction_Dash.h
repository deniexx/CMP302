// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_Dash.generated.h"

class UCExtendedCharacterMovement;
class ACCommonCharacter;
class AStaticMeshActor;
class UCDashUserWidget;

/**
 * 
 */
UCLASS()
class CMP302_API UCAction_Dash : public UCAction
{
	GENERATED_BODY()
	
public:
	
	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;

	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

	virtual void TickAction_Implementation(float DeltaTime) override;

	virtual void StartAction_Implementation(AActor* InInstigator) override;

	virtual void StopAction_Implementation(AActor* InInstigator) override;

	virtual bool CanStart_Implementation(AActor* InInstigator) override;

	/**
	 * Gets the dash timer progress in a range of 0 to 1
	 * @return The dash timer progress in a value of 0 to 1
	 */
	float GetDashTimerProgress() const;
	
protected:

	virtual FString GetInCooldownMessage() const override;

	UPROPERTY()
	ACCommonCharacter* Character;

	UPROPERTY()
	UCExtendedCharacterMovement* MovementComponent;

	/** The overcharge tag to search for, this is used to determine if you can slash during the dash */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FGameplayTag OverchargeTag;

	/** The duration of the time slow while holding the dash key */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashTimeSlowDuration;

	/** The distance to dash forward */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashDistance;

	/** The velocity to be applied after the dash */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float VelocityAfterDash;

	/** Dash Visual Effects */
	UPROPERTY()
	AStaticMeshActor* DashIndicatorActor;

	/** A mesh indicating the end position of the dash */
	UPROPERTY(EditDefaultsOnly, Category = "Dash Visuals")
	UStaticMesh* DashIndicatorMesh;

	/** The instance of the Widget for the dash */
	UPROPERTY()
	UCDashUserWidget* DashVisualOverlayInstance;

	/** The Class of the Widget for the dash */
	UPROPERTY(EditDefaultsOnly, Category = "Dash Visuals")
	TSubclassOf<UCDashUserWidget> DashVisualOverlayInstanceClass;

	/** The instance of the widget used to display the dash cooldown */
	UPROPERTY()
	UUserWidget* DashCooldownWidgetInstance;

	/** The class of the dash cooldown widget */
	UPROPERTY(EditDefaultsOnly, Category = "Dash Visuals")
	TSubclassOf<UUserWidget> DashCooldownWidgetClass;

	/** The material parameters used to drive the UI */
	UPROPERTY(EditDefaultsOnly, Category = "Dash Visuals")
	UMaterialParameterCollection* UIMaterialParameters;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* DashSound;

	/** Interrupts the dash and resets it if player is on the ground */
	UFUNCTION()
	void InterruptDash();

	/** Is the character dashing */
	bool bDashing;

	/** Has the character touched the ground since the last dash */
	bool bGroundTouched;

	/** The end location of the dash */
	FVector DashLocation;

	/** Default variables that we can return to */
	float DefaultAirControl;
	float DefaultMaxWalkSpeed;
	float DefaultMaxAcceleration;

	/**
	 * Performs the actual dash
	 */
	void Dash();

	/**
	 * Here we reset the dash variables, but do not reset bDashConsumed, to do that we need to use RefillDash()
	 */
	void ResetDash();

	UFUNCTION()
	void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMovementMode);

	UPROPERTY()
	FTimerHandle DashTimerHandle;
	
	void TweenDashCooldownUIParameter(float Value);

	UFUNCTION(BlueprintCallable)
	void GrantDashCharge();
};
