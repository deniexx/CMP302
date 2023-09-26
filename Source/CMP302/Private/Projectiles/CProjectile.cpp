// Coded by: Denis Dimitrov for CMP302


#include "Projectiles/CProjectile.h"

#include "ActorComponents/CCombatComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "System/CGameplayFunctionLibrary.h"

// Sets default values
ACProjectile::ACProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName("Projectile");
	SetRootComponent(SphereComponent);
	
	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComponent"));
	EffectComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false; // We might need to set this to false, in order to get the shuriken to rotate properly
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	LifeSpan = 5.f;
	Pierce = 1;
	PiercedEnemies = 0;
}

void ACProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetLifeSpan(LifeSpan);

	if (AudioComponent)
		AudioComponent->Play();
}

void ACProjectile::SetAttackStatus(EAttackStatusType NewAttackStatus)
{
	AttackStatus = NewAttackStatus;
	OnAttackStatusUpdated();
}

void ACProjectile::OnAttackStatusUpdated_Implementation()
{
	// @NOTE: This should be implemented in blueprint
}

void ACProjectile::PlayExtrasOnHit() const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorLocation());
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), HitCameraShake, GetActorLocation(), 00.0f, 800.0f, 0.0f);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
}

void ACProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	/** Check if we have hit an actor and if they are not ourselves */
	if (!Other) return;
	if (Other == GetInstigator()) return;

	PlayExtrasOnHit();
	Destroy();

	FAttackData AttackData;
	AttackData.Instigator = GetInstigator();
	AttackData.AttackStatusType = AttackStatus;
	/** This bool can be used to determine whether we successfully hit the character, i.e. if the hit was registered
	 * meaning they were either the same AttackStatus, or we had a White(GodType) attack status */
	bool bSuccess = UCGameplayFunctionLibrary::TryRegisterHit(AttackData, Other);
	++PiercedEnemies;
	PlayExtrasOnHit();
	
	/** Destroy the projectile if we have hit the maximum number of enemies */
	if (Pierce >= PiercedEnemies)
	{
		Destroy();
	}
}
