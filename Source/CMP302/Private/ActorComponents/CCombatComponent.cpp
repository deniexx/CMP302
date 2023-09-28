// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CCombatComponent.h"

#include "Character/CCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"
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
	PrimaryComponentTick.bCanEverTick = true;
	bCanBeHit = true;
	bIsSlashAttackReady = true;
	bCanFireProjectile = true;

	FireProjectileDelay = 1.25f;
	SlashAttackAnimDelay = 0.25f;
	ColorLerpAlpha = 0.f;
	ColorLerpSpeed = 1.f;
}

void UCCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/** Lerp smoothly between the previous and the current attack status color */
	if (ColorLerpAlpha < 1)
	{
		const FLinearColor Color = FMath::Lerp(PreviousAttackStatusColor, CurrentAttackStatusColor, ColorLerpAlpha);
		UKismetMaterialLibrary::SetVectorParameterValue(CharacterOwner, PlayerMaterialParameters,
				TEXT("PlayerAttackStatusColor"), Color);

		ColorLerpAlpha += DeltaTime * ColorLerpSpeed;
	}
}

void UCCombatComponent::Init(UStaticMeshComponent* InShurikenMesh)
{
	CharacterOwner = GetOwner<ACCharacter>();
	ShurikenMesh = InShurikenMesh;
	ShurikenMesh->SetVisibility(true);
	if (AttackStatusType == EAttackStatusType::None)
	{
		const APlayerController* Controller = CharacterOwner ? CharacterOwner->GetController<APlayerController>() : nullptr;
		const bool bIsPlayerControlled = Controller != nullptr;
		
		if (!bIsPlayerControlled)
		{
			// Get a random index from the range to use in the enum as red, green or blue
			int index = FMath::RandRange(0, 2) + 1;
			UpdateAttackStatusType(EAttackStatusType(index));
		}
		else
		{
			UpdateAttackStatusType(EAttackStatusType::Red);
		}
	}
}

void UCCombatComponent::FireProjectile()
{
	if (!bCanFireProjectile) return;
	
	if (FireProjectileMontage)
	{
		if (UAnimInstance* AnimInstance = CharacterOwner->GetMesh1P()->GetAnimInstance())
		{
			bCanFireProjectile = false;
			AnimInstance->Montage_Play(FireProjectileMontage);
		}
	}
}

void UCCombatComponent::SlashAttack()
{
	if (!bIsSlashAttackReady) return;
	
	const int index = FMath::RandRange(0, SlashAttackMontages.Num() - 1);

	if (UAnimMontage* AnimMontage = SlashAttackMontages[index])
	{
		if (UAnimInstance* AnimInstance = CharacterOwner->GetMesh1P()->GetAnimInstance())
		{
			bIsSlashAttackReady = false;
			CharacterOwner->SwitchSwordCollision(true);
			AnimInstance->Montage_Play(AnimMontage);
		}
	}
}

void UCCombatComponent::UpdateAttackStatusType(EAttackStatusType NewAttackStatusType)
{
	const EAttackStatusType Previous = AttackStatusType;
	AttackStatusType = NewAttackStatusType;

	OnAttackStatusTypeUpdated.Broadcast(Previous, AttackStatusType);

	ColorLerpAlpha = 0.f;
	PreviousAttackStatusColor =  UKismetMaterialLibrary::GetVectorParameterValue(CharacterOwner, PlayerMaterialParameters, TEXT("PlayerAttackStatusColor"));
	CurrentAttackStatusColor = UCGameplayFunctionLibrary::GetColorFromAttackStatus(AttackStatusType);
}

bool UCCombatComponent::CheckCanBeHit(const FAttackData& AttackData) const
{
	const bool bGodMode = CVarGodMode.GetValueOnAnyThread() > 0;
	
	if (bGodMode || !bCanBeHit)
	{
		// We can't be hit as we are currently in GodMode or we can't be hit (we might be dashing, etc...)
		return false;
	}

	const APlayerController* Controller = CharacterOwner ? CharacterOwner->GetController<APlayerController>() : nullptr;
	const bool bIsPlayerControlled = Controller != nullptr;
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

void UCCombatComponent::SpawnProjectile()
{
	if (ensureAlways(ProjectileClass))
	{
		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(CharacterOwner);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CharacterOwner->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (CharacterOwner->GetControlRotation().Vector() * 5000);

		FHitResult HitResult;
		if (GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = HitResult.ImpactPoint;
		}
		
		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - TraceStart).Rotator();
		TraceStart = (CharacterOwner->GetActorForwardVector() * 100.f) + TraceStart;
		TraceStart.Z -= 10;
		
		USkeletalMeshComponent* Mesh1P = CharacterOwner->GetMesh1P();
		FVector SpawnLocation = Mesh1P ? Mesh1P->GetSocketLocation("hand_lSocket") : TraceStart;
		
		FTransform SpawnTransform = FTransform(ProjectileRotation, SpawnLocation);
		ACProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACProjectile>(ProjectileClass, SpawnTransform,
						CharacterOwner, CharacterOwner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->FinishSpawning(SpawnTransform);
		Projectile->SetAttackStatus(AttackStatusType);

		if (ShurikenMesh)
			ShurikenMesh->SetVisibility(false);
		
		GetWorld()->GetTimerManager().SetTimer(FireProjectileTimerHandle, this, &UCCombatComponent::FireProjectile_TimerElapsed, FireProjectileDelay);
	}
}

void UCCombatComponent::FireProjectile_TimerElapsed()
{
	bCanFireProjectile = true;
	if (ShurikenMesh)
		ShurikenMesh->SetVisibility(true);
	
	FireProjectileTimerHandle.Invalidate();
}

void UCCombatComponent::ResetSlashAttack()
{
	CharacterOwner->SwitchSwordCollision(false);
	
	GetWorld()->GetTimerManager().SetTimer(SlashAttackTimerHandle, this, &UCCombatComponent::SlashAttack_TimerElapsed, SlashAttackAnimDelay);
}

void UCCombatComponent::SlashAttack_TimerElapsed()
{
	bIsSlashAttackReady = true;
	SlashAttackTimerHandle.Invalidate();
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
	OnHitTaken.Broadcast(AttackData);

	return true;
}

UMaterialParameterCollection* UCCombatComponent::GetPlayerMaterialParameters() const
{
	return PlayerMaterialParameters;
}

EAttackStatusType UCCombatComponent::GetAttackStatusType() const
{
	return AttackStatusType;
}
