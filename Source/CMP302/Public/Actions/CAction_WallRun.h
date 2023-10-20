// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_WallRun.generated.h"

class ACCommonCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class CMP302_API UCAction_WallRun : public UCAction
{
	GENERATED_BODY()

public:

	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;

	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

	virtual void StartAction_Implementation(AActor* InInstigator) override;

	virtual void StopAction_Implementation(AActor* InInstigator) override;

	virtual bool CanStart_Implementation(AActor* InInstigator) override;

	virtual void TickAction_Implementation(float DeltaTime) override;

protected:

	virtual FString GetInCooldownMessage() const override;

private:

	/** The velocity to be applied after jumping off a wall during wall run */
	UPROPERTY(EditAnywhere, Category = "Traversal", meta = (AllowPrivateAccess = "true"))
	float WallRunJumpOffVelocity;

	/** The object types to query for a wall run collision check */
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	TArray<TEnumAsByte<EObjectTypeQuery>> WallRunObjectTypes;

	/** The tags for wall running (these are added/removed depending on the action state)  */
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	FGameplayTagContainer WallRunningTags;

	/** The tags for when we are in air */
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	FGameplayTagContainer InAirTags;

	/** Camera Modifier to be applied, during the a wall run */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraModifier> LeftWallRunCameraModifier;

	/** Camera Modifier to be applied, during the a wall run */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraModifier> RightWallRunCameraModifier;

	UPROPERTY()
	ACCommonCharacter* Character;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	/** Is the character wall running */
	bool bWallRunning = false;
	
	UPROPERTY()
	UCameraModifier* AppliedCameraModifierLeft;

	UPROPERTY()
	UCameraModifier* AppliedCameraModifierRight;

	/** Was the last wall run on the left (used for camera modifiers) */
	bool bWasLastWallRunOnLeft;

	UPROPERTY()
	FHitResult WallRunHit;

	/** Starts the wall running behaviour */
	void BeginWallRun();

	/** Ends the wall running behaviour */
	void EndWallRun();

	/** Finds a wall runnable wall, this is used to either find a wall to initiate a wall run or to check if we can continue the wall run with the current wall */
	bool FindRunnableWall(FHitResult& OutWallHit) const;

	/** Checks if the wall run is on the left */
	bool IsWallOnTheLeft(const FHitResult& InWallHit);

	UFUNCTION()
	void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);
};
