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

	float GetDashTimerProgress() const;
	
protected:

	virtual FString GetInCooldownMessage() const override;

	UPROPERTY()
	ACCommonCharacter* Character;

	UPROPERTY()
	UCExtendedCharacterMovement* MovementComponent;

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
	
	UPROPERTY(EditDefaultsOnly, Category = "Dash Visuals", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* DashIndicatorMesh;

	UPROPERTY()
	UCDashUserWidget* DashVisualOverlayInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Dash Visuals", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCDashUserWidget> DashVisualOverlayInstanceClass;

	UFUNCTION()
	void InterruptDash();

	bool bDashing;

	bool bGroundTouched;

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
};
