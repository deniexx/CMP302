// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Character/CCommonCharacter.h"
#include "CPlayerCharacter.generated.h"

class UCInteractionComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USkeletalMeshComponent;

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerHit, int32, RoomIndex);

/**
 * 
 */
UCLASS()
class CMP302_API ACPlayerCharacter : public ACCommonCharacter
{
	GENERATED_BODY()

public:

	ACPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	FOnPlayerHit OnPlayerHit;

protected:

	virtual void BeginPlay() override;

	/** Called for movement input */
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	/** Called when we want to fire a projectile */
	UFUNCTION()
	void FireProjectile(const FInputActionValue& Value);

	/** Called when we want to switch attack status */
	UFUNCTION()
	void SwitchAttackStatus(const FInputActionValue& Value);

	UFUNCTION()
	void BeginDash(const FInputActionValue& Value);

	UFUNCTION()
	void EndDash(const FInputActionValue& Value);

	UFUNCTION()
	void SlashAttack(const FInputActionValue& Value);

	UFUNCTION()
	void BeginTraversal(const FInputActionValue& Value);

	UFUNCTION()
	void EndTraversal(const FInputActionValue& Value);

	UFUNCTION()
	void BeginSlide(const FInputActionValue& Value);

	UFUNCTION()
	void EndSlide(const FInputActionValue& Value);

	UFUNCTION()
	void OverloadCharacter(const FInputActionValue& Value);

	UFUNCTION()
	void Interact(const FInputActionValue& Value);

private:
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCInteractionComponent* InteractionComponent;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpInputAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveInputAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookInputAction;

	/** Fire Projectile Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* FireProjectileInputAction;

	/** Switch Attack Status Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchAttackStatusInputAction;

	/** Dash Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* DashInputAction;

	/** Slash Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SlashInputAction;

	/** Slash Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SlideInputAction;

	/** Overload Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* OverloadInputAction;

	/** Overload Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractInputAction;


	/** Fire Projectile Action Tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FGameplayTag FireProjectileActionTag;

	/** Fire Projectile Action Tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FGameplayTag SwordSlashActionTag;

	/** Dash Action Tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FGameplayTag DashActionTag;

	/** Traversal Action Tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FGameplayTag TraversalActionTag;
	
	/** Traversal Action Tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FGameplayTag SlideActionTag;

	/** Overload Action Tag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FGameplayTag OverloadActionTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FGameplayTag InputBlockTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer InputBlockingTags;

	FTransform SpawnTransform;

	bool bResetTransform;

	int32 RoomIndex;

	virtual void OnHitTaken(const FAttackData& AttackData) override;

	virtual void ReadyActor() override;

	virtual void TweenAppearance(float Value) override;

	virtual void PawnClientRestart() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Returns FirstPersonCameraComponent sub object */
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable)
	void SetSpawnTransform(const FTransform& InSpawnTransform);
};
