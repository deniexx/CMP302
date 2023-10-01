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

private:

	UPROPERTY(EditAnywhere, Category = "Traversal", meta = (AllowPrivateAccess = "true"))
	float WallRunJumpOffVelocity;

	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	TArray<TEnumAsByte<EObjectTypeQuery>> WallRunObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	FGameplayTagContainer WallRunningTags;

	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	FGameplayTagContainer InAirTags;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraModifier> LeftWallRunCameraModifier;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraModifier> RightWallRunCameraModifier;

	UPROPERTY()
	ACCommonCharacter* Character;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	
	bool bWallRunning = false;
	
	UPROPERTY()
	UCameraModifier* AppliedCameraModifier;

	UPROPERTY()
	FHitResult WallRunHit;

	void BeginWallRun();
	
	void EndWallRun();

	bool FindRunnableWall(FHitResult& OutWallHit) const;

	bool IsWallOnTheLeft(const FHitResult& InWallHit) const;

	UFUNCTION()
	void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);
};
