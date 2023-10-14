// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_Shuriken.h"

#include "ActorComponents/CCombatStatusComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/CCommonCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Projectiles/CProjectile.h"
#include "System/CGameplayFunctionLibrary.h"

void UCAction_Shuriken::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	ACCommonCharacter* PlayerCharacter = Cast<ACCommonCharacter>(InInstigator);
	
	ShurikenMeshComponent = Cast<UStaticMeshComponent>(InInstigator->AddComponentByClass(UStaticMeshComponent::StaticClass(), true, FTransform(), false));
	ShurikenMeshComponent->SetStaticMesh(ShurikenMesh);
	ShurikenMeshComponent->SetMobility(EComponentMobility::Movable);
	ShurikenMeshComponent->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_l"));
	ShurikenMeshComponent->SetRelativeTransform(AttachTransform);
	ShurikenMeshComponent->bCastDynamicShadow = false;
	ShurikenMeshComponent->CastShadow = false;
	ShurikenMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShurikenMeshComponent->IgnoreActorWhenMoving(PlayerCharacter, true);
	ShurikenMeshComponent->SetVisibility(false);

	ShurikenCooldownWidget = CreateWidget(PlayerCharacter->GetController<APlayerController>(), ShurikenCooldownWidgetClass);
	ShurikenCooldownWidget->AddToViewport();
}

void UCAction_Shuriken::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);

	ShurikenMeshComponent->DestroyComponent();

	ShurikenCooldownWidget->RemoveFromParent();
}

void UCAction_Shuriken::TickAction_Implementation(float DeltaTime)
{
	Super::TickAction_Implementation(DeltaTime);

	UKismetMaterialLibrary::SetScalarParameterValue(GetOuter(), UIParameterCollection, "ShurikenTimerProgress", 1 - GetRemainingCooldownFraction());
}

void UCAction_Shuriken::StartAction_Implementation(AActor* InInstigator)
{
	Super::StartAction_Implementation(InInstigator);

	ACCommonCharacter* Character = Cast<ACCommonCharacter>(InInstigator);
	Character->PlayAnimMontage(ShurikenThrowMontage);

	ShurikenMeshComponent->SetVisibility(true);
	FTimerHandle AttackDelayHandle;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "AttackDelayTimer_Elapsed", Character);

	GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, Delegate, AttackAnimDelay, false);
}

FString UCAction_Shuriken::GetInCooldownMessage() const
{
	return FString::Printf(TEXT("%ls module not ready!"), *ActionName.ToString());
}

void UCAction_Shuriken::AttackDelayTimer_Elapsed(ACCommonCharacter* Character)
{
	if (ensureAlways(ShurikenProjectileClass))
	{
		ACProjectile* Projectile = UCGameplayFunctionLibrary::SpawnProjectile(Character, ShurikenProjectileClass, Character);

		const UCCombatStatusComponent* CombatComponent = UCCombatStatusComponent::GetCombatStatusComponent(Character);
		const EAttackStatusType StatusToSet = CombatComponent->GetAttackStatusType();
		Projectile->SetAttackStatus(StatusToSet);

		if (ShurikenMeshComponent)
			ShurikenMeshComponent->SetVisibility(false);
	}

	const FString Message = FString::Printf(TEXT("%s thrown."), *ActionName.ToString());
	UCGameplayFunctionLibrary::AddStatusReportMessage(GetOuter(), Message);
}
