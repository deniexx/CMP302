// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "GameFramework/Actor.h"
#include "CProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class CMP302_API ACProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACProjectile();
	
	virtual void PostInitializeComponents() override;

	/**
	 * Sets the new attack status to this projectile
	 * @param NewAttackStatus New Attack Status to be applied, this will also update the material of the projectile
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetAttackStatus(EAttackStatusType NewAttackStatus);

	/**
	 * Stops the projectile in place and disables its collision
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopAndDisableCollision();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	/** How long should the projectile fly for */
	UPROPERTY(EditAnywhere, Category = "Stats")
	float LifeSpan;

	/** How many enemies can the projectile pierce */
	UPROPERTY(EditAnywhere, Category = "Stats")
	uint8 Pierce;

	/** The strength of the impact */
	UPROPERTY(EditAnywhere, Category = "Stats")
	float ImpactStrength;

	/** How many enemies have been pierced */
	uint8 PiercedEnemies;

	/** Our current attack status */
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	EAttackStatusType AttackStatus;

	/** Fly-by sound to be played by this audio component */
	UPROPERTY(VisibleAnywhere, Category = "Extras|Sounds")
	UAudioComponent* AudioComponent;

	/** Sounds to be played when something was hit */
	UPROPERTY(EditAnywhere, Category = "Extras|Sounds")
	USoundBase* HitSound;

	/** Particle System to play when something was hit */
	UPROPERTY(EditAnywhere, Category = "Extras|VFX")
	UParticleSystem* HitParticles;

	/** Camera Shake to be played when something was hit */
	UPROPERTY(EditAnywhere, Category = "Extras|VFX")
	TSubclassOf<UCameraShakeBase> HitCameraShake;

protected:

	virtual void BeginPlay() override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void OnAttackStatusUpdated();
	
	/**
	 * Plays the visual and sound extras for this projectile upon colliding(hitting) with something
	 */
	void PlayExtrasOnHit() const;
};
