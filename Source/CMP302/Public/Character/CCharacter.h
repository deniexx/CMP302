// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CCharacter.generated.h"

class USpringArmComponent;
class UCCharacterMovementComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UMaterialParameterCollection;
class USceneComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UCCombatComponent;

UCLASS()
class CMP302_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACCharacter(const FObjectInitializer& ObjectInitializer);

	/* -----------------------------------------------------
	 *							COMPONENTS	
	 * -----------------------------------------------------
	 */
private:
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	/** Combat Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UCCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UCCharacterMovementComponent* ExtendedMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SwordMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ShurikenMesh;
	
	/* -----------------------------------------------------
	 *						     PROPERTIES
	 * -----------------------------------------------------
	 */
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Fire Projectile Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* FireProjectileAction;

	/** Switch Attack Status Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchAttackStatusAction;

	/** Dash Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	/** Slash Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SlashAction;

	FTransform MeshTransform;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

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

	UFUNCTION(BlueprintNativeEvent)
	void OnHitTaken_BP(const FAttackData& AttackData);

	UFUNCTION(BlueprintNativeEvent)
	void InitAI();

	UFUNCTION(BlueprintNativeEvent)
	void TickAI(float DeltaSeconds);

private:

	UFUNCTION()
	void OnHitTaken(const FAttackData& AttackData);

	UFUNCTION()
	void OnSwordHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MeshDynamicMaterials;

	bool bDead;

	float AppearanceAlpha;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns Mesh1P sub object */
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	/** Returns FirstPersonCameraComponent sub object */
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/**
	 * Switches the sword collision between Query Only and No Collision
	 * @param bEnabled Should the collision be enabled
	 */
	void SwitchSwordCollision(bool bEnabled) const;

	/**
	 * Sets up the actor to be ready, this can include reviving its
	 */
	void ReadyActor();

	/**
	 * Check if the actor is dead
	 * @return Whether the actor is dead
	 */
	bool IsDead() const;
};
