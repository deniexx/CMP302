// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_Slide.generated.h"

class ACPlayerCharacter;
class UCExtendedCharacterMovement;
/**
 * 
 */
UCLASS()
class CMP302_API UCAction_Slide : public UCAction
{
	GENERATED_BODY()

public:

	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;

	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

	virtual void TickAction_Implementation(float DeltaTime) override;
	
	virtual void StartAction_Implementation(AActor* InInstigator) override;

	virtual void StopAction_Implementation(AActor* InInstigator) override;

	virtual bool CanStart_Implementation(AActor* InInstigator) override;

protected:

	/** Crouching tags to add/remove depending on the action */
	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTagContainer CrouchingTags;

	/** The tag for when we are in the air (used for calculating the duration of the slide) */
	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTag InAirTag;

	/** The force to be applied when sliding */
	UPROPERTY(EditAnywhere, Category = "Forces")
	float SlideForce;

	/** The duration on how long to slide */
	UPROPERTY(EditAnywhere, Category = "Forces")
	float OnGroundSlideDuration;

	/** A camera modifier to be applied when sliding */
	UPROPERTY(EditAnywhere, Category = "Visuals")
	TSubclassOf<UCameraModifier> CameraModifier;

	UPROPERTY()
	UCameraModifier* AppliedCameraModifier;

	/** The max speed of the slide */
	float MaxSlideSpeed;
	
	UPROPERTY()
	ACPlayerCharacter* Character;

	UPROPERTY()
	UCExtendedCharacterMovement* MovementComponent;
	
	FVector VelocityDirection;
	
	float CurrentSlideDuration;
	
	float DefaultMaxWalkSpeed;

	float StandingCapsuleHalfHeight;

	float CrouchAlpha;

	bool bCrouching;

	bool bSliding;

	/** Starts the actual sliding behaviour */
	void BeginSliding();

	/** Gets a multiplier based on the slope */
	float GetSlopeEffect(const FVector& FloorNormal) const;

	/** Checks if the character can uncrouch */
	bool CanUncrouch() const;

	/** Is the character on the ground */
	bool IsOnGround() const;

	/** Ends the sliding behaviour */
	void EndSliding();
};
