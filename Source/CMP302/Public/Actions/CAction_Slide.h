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
	
	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTagContainer CrouchingTags;

	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTag InAirTag;

	UPROPERTY(EditAnywhere, Category = "Forces")
	float SlideForce;

	UPROPERTY(EditAnywhere, Category = "Forces")
	float OnGroundSlideDuration;
	
	float MaxSlideSpeed;

	UPROPERTY()
	ACPlayerCharacter* Character;

	UPROPERTY()
	UCExtendedCharacterMovement* MovementComponent;

	float CurrentSlideDuration;
	
	float DefaultMaxWalkSpeed;

	float StandingCapsuleHalfHeight;

	float CrouchAlpha;

	bool bCrouching;

	bool bSliding;

	void BeginSliding();

	bool IsFloorDownwardSlope(const FVector& FloorNormal);
	
	bool CanUncrouch() const;

	bool IsOnGround() const;

	void EndSliding();
};
