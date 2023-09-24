// Coded by: Denis Dimitrov for CMP302


#include "System/CCheatManager.h"

#include "ActorComponents/CCombatComponent.h"

void UCCheatManager::HitTarget(EAttackStatusType AttackStatus)
{
	if (APlayerController* PlayerController = GetOuterAPlayerController())
	{
		FHitResult TargetHitResult;
		const AActor* TargetActor = GetTarget(PlayerController, TargetHitResult);

		if (UCCombatComponent* CombatComponent = UCCombatComponent::GetCombatComponent(TargetActor))
		{
			FAttackData AttackData;
			AttackData.AttackStatusType = AttackStatus;
			AttackData.Instigator = PlayerController->GetPawn();
			CombatComponent->TryRegisterHit(AttackData);
		}
	}
}
