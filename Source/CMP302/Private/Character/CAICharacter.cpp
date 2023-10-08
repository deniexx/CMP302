// Coded by: Denis Dimitrov for CMP302


#include "Character/CAICharacter.h"

#include "ActorComponents/CCombatStatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/CGameplayFunctionLibrary.h"

ACAICharacter::ACAICharacter()
{
	
}

void ACAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	for (UMaterialInstanceDynamic* Material : MeshDynamicMaterials)
	{
		Material->SetVectorParameterValue("LogoLayer0_Color", UCGameplayFunctionLibrary::GetColorFromAttackStatus(CombatComponent->GetAttackStatusType()));
		Material->SetVectorParameterValue("LogoLayer1_Color", UCGameplayFunctionLibrary::GetColorFromAttackStatus(CombatComponent->GetAttackStatusType()));
		Material->SetVectorParameterValue("LogoLayer2_Color", UCGameplayFunctionLibrary::GetColorFromAttackStatus(CombatComponent->GetAttackStatusType()));
	}

	MeshTransform = GetMesh()->GetComponentTransform();

	InitAI();
}

void ACAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TickAI(DeltaTime);
}

void ACAICharacter::OnHitTaken(const FAttackData& AttackData)
{
	Super::OnHitTaken(AttackData);

	bDead = true;

	StartTweenAppearance();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCharacterMovement()->DisableMovement();
	
	if (AttackData.Instigator)
	{
		FVector ImpulseDirection = (GetActorLocation() - AttackData.Instigator->GetActorLocation());
		ImpulseDirection.Normalize();
		GetMesh()->AddImpulse(ImpulseDirection * AttackData.ImpactStrength);
	}
}

void ACAICharacter::TweenAppearance(float Value)
{
	Super::TweenAppearance(Value);

	if (bDead && Value <= 0.f)
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionProfileName("NoCollision");
	}
}

void ACAICharacter::ReadyActor()
{
	Super::ReadyActor();

	GetMesh()->SetWorldTransform(MeshTransform);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	GetMesh()->SetWorldTransform(MeshTransform);
	GetMesh()->SetMobility(EComponentMobility::Movable);
		
	InitAI();
}

void ACAICharacter::TickAI_Implementation(float DeltaSeconds)
{
	
}

void ACAICharacter::InitAI_Implementation()
{
	
}
