// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_FireProjectileAI.h"

#include "Character/CCommonCharacter.h"
#include "Projectiles/CProjectile.h"
#include "System/CGameplayFunctionLibrary.h"

void UCAction_FireProjectileAI::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	ACCommonCharacter* Character = Cast<ACCommonCharacter>(InInstigator);
	
	ACProjectile* Projectile = UCGameplayFunctionLibrary::SpawnProjectile(Character, ProjectileClass, Character);
	Projectile->SetAttackStatus(EAttackStatusType::Enemy);

	StopAction(InInstigator);
}
