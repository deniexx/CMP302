// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CCombatComponent.h"

#include "Character/CCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Projectiles/CProjectile.h"
#include "System/CGameplayFunctionLibrary.h"

static TAutoConsoleVariable<int32> CVarGodMode(
	TEXT("GodMode"),
	0,
	TEXT("Draws debug info about traversal")
	TEXT(" 0: Can not be hit (God Mode)/n")
	TEXT(" 1: Can be hit (Normal Mode)/n"),
	ECVF_Cheat
);

UCCombatComponent* UCCombatComponent::GetCombatComponent(const AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCCombatComponent>(FromActor->GetComponentByClass(StaticClass()));
	}

	return nullptr;
}

// Sets default values for this component's properties
UCCombatComponent::UCCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bCanBeHit = true;

	FireProjectileAnimDelay = 0.2f;
}

void UCCombatComponent::Init()
{
	if (AttackStatusType == EAttackStatusType::None)
	{
		const ACharacter* Owner = GetOwner<ACharacter>();
		const bool bIsPlayerControlled = Owner ? Owner->GetController()->IsPlayerController() : false;
		
		if (!bIsPlayerControlled)
		{
			// Get a random index from the range to use in the enum as red, green or blue
			int index = FMath::RandRange(0, 2) + 1;
			UpdateAttackStatusType(EAttackStatusType(index));
		}
	}
}

void UCCombatComponent::UpdateAttackStatusType(EAttackStatusType NewAttackStatusType)
{
	const EAttackStatusType Previous = AttackStatusType;
	AttackStatusType = NewAttackStatusType;

	OnAttackStatusTypeUpdated.Broadcast(Previous, AttackStatusType);

	UKismetMaterialLibrary::SetVectorParameterValue(GetOwner(), PlayerMaterialParameters,
		TEXT("PlayerAttackStatusColor"), FLinearColor::FromSRGBColor(UCGameplayFunctionLibrary::GetColorFromAttackStatus(NewAttackStatusType)));
}

bool UCCombatComponent::CheckCanBeHit(const FAttackData& AttackData) const
{
	const bool bGodMode = CVarGodMode.GetValueOnAnyThread() > 0;
	
	if (bGodMode || !bCanBeHit)
	{
		// We can't be hit as we are currently in GodMode or we can't be hit (we might be dashing, etc...)
		return false;
	}
	
	const ACharacter* Owner = GetOwner<ACharacter>();
	const bool bIsPlayerControlled = Owner ? Owner->GetController()->IsPlayerController() : false;
	const bool bAlwaysHit = AttackData.AttackStatusType == EAttackStatusType::White;
	
	if (!bIsPlayerControlled)
	{
		if (AttackData.AttackStatusType != AttackStatusType && !bAlwaysHit)
		{
			// We can't be hit as the attack status types are not the same
			return false;
		}
	}
	else
	{
		if (AttackData.AttackStatusType != EAttackStatusType::Enemy)
		{
			// We can't be hit as this is not an enemy attack
			return false;
		}
	}

	// If we have reached this point, it means the other checks have failed and we can be hit
	return true;
}

void UCCombatComponent::FireProjectile()
{
	// This means that the timer is currently running, and we don't want to re run it
	if (FireProjectileTimerHandle.IsValid())
		return;

	ACharacter* Owner = GetOwner<ACharacter>();
	if (FireProjectileMontage && Owner)
	{
		Owner->PlayAnimMontage(FireProjectileMontage);
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "FireProjectile_TImerElapsed");

	GetWorld()->GetTimerManager().SetTimer(FireProjectileTimerHandle, TimerDelegate, FireProjectileAnimDelay, false);
}

void UCCombatComponent::FireProjectile_TimerElapsed()
{
	FireProjectileTimerHandle.Invalidate();
	
	if (ensureAlways(ProjectileClass))
	{
		ACCharacter* InstigatorCharacter = GetOwner<ACCharacter>(); 
		
		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);

		FHitResult HitResult;
		if (GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = HitResult.ImpactPoint;
		}

		/** @TODO: Once we have the arms model, we can use the socket location as starting location */
		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - TraceStart).Rotator();
		TraceStart = (InstigatorCharacter->GetActorForwardVector() * 100.f) + TraceStart;
		TraceStart.Z -= 10;
		FTransform SpawnTransform = FTransform(ProjectileRotation, TraceStart);
		
		ACProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACProjectile>(ProjectileClass, SpawnTransform,
						InstigatorCharacter, InstigatorCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->FinishSpawning(SpawnTransform);
		Projectile->SetAttackStatus(AttackStatusType);
	}
}

bool UCCombatComponent::TryRegisterHit(const FAttackData& AttackData) const
{
	const bool bCanRegisterHit = CheckCanBeHit(AttackData);

	// If we can't be hit, return false and exit the function
	if (!bCanRegisterHit)
	{
		return bCanRegisterHit;
	}

	// @TODO: Potentially claim a kill if we implement an interface (can be used to track certain statistics)
	OnHitTaken.Broadcast();

	return true;
}
