// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CCharacterMovementComponent.generated.h"

class AStaticMeshActor;
class UCDashUserWidget;

/**
 * 
 */
UCLASS()
class CMP302_API UCCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UCCharacterMovementComponent();

	/**
	 * Begins the dash mechanic, starts the slow motion and waits for the key to be released
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void BeginDash();

	/**
	 * Ends the dash, which also performs the dash
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void EndDash();

	/**
	 * Interrupts the dash, meaning the character will stop the slow motion, but will not dash
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void InterruptDash();

	/**
	 * Refills the dash and resets it so that it's ready to be used
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RefillDash();

	UFUNCTION(BlueprintCallable, Category = "Timers")
	float GetDashTimerProgress() const;

protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** The duration of the time slow while holding the dash key */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashTimeSlowDuration;

	/** The distance to dash forward */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashDistance;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;


private:

	/** Dash Visual Effects */
	UPROPERTY()
	AStaticMeshActor* DashIndicatorActor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Dash Visuals", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* DashIndicatorMesh;

	UPROPERTY()
	UCDashUserWidget* DashVisualOverlayInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Dash Visuals", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCDashUserWidget> DashVisualOverlayInstanceClass;
	
	/** Dash states */
	bool bDashConsumed;
	bool bDashing;

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
	
	FTimerHandle DashTimerHandle;
};
