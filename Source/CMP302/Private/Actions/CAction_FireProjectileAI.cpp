// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_FireProjectileAI.h"

#include "Character/CCommonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/CProjectile.h"
#include "System/CGameplayFunctionLibrary.h"

void UCAction_FireProjectileAI::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	ACCommonCharacter* Character = Cast<ACCommonCharacter>(InInstigator);
	const AActor* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(Character, 0);
	FVector HitLocation = PlayerCharacter->GetActorLocation();
	HitLocation.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	ACProjectile* Projectile = UCGameplayFunctionLibrary::SpawnProjectileWithStarAndEndPosition(Character, ProjectileClass, Character, Character->GetActorLocation(), HitLocation);
	Projectile->SetAttackStatus(EAttackStatusType::Enemy);

	StopAction(InInstigator);
}
