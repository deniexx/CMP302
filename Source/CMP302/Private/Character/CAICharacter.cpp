// Coded by: Denis Dimitrov for CMP302


#include "Character/CAICharacter.h"

#include "ActorComponents/CCombatStatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/CGameplayFunctionLibrary.h"

ACAICharacter::ACAICharacter()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_r"));
	WeaponMesh->SetRelativeLocation(FVector(-17.f, -6.f, -11.5f));
	WeaponMesh->SetRelativeRotation(FRotator(9.5f, 185.f, 2.6f));


	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(GetMesh());
}

void ACAICharacter::BeginPlay()
{
	Super::BeginPlay();
	WeaponMesh->SetVisibility(false);
	GetMesh()->SetVisibility(false);
	
	for (UMaterialInstanceDynamic* Material : MeshDynamicMaterials)
	{
		Material->SetVectorParameterValue("LogoLayer0_Color", UCGameplayFunctionLibrary::GetColorFromAttackStatus(CombatComponent->GetAttackStatusType()));
		Material->SetVectorParameterValue("LogoLayer1_Color", UCGameplayFunctionLibrary::GetColorFromAttackStatus(CombatComponent->GetAttackStatusType()));
		Material->SetVectorParameterValue("LogoLayer2_Color", UCGameplayFunctionLibrary::GetColorFromAttackStatus(CombatComponent->GetAttackStatusType()));
	}

	UTweenSubsystem* TweenSubsystem = GetGameInstance()->GetSubsystem<UTweenSubsystem>();
	FTweenHandle TweenHandle;
	FTweenDynamicDelegate Delegate;
	Delegate.BindDynamic(this, &ThisClass::TweenDrone);

	TweenSubsystem->AddTween(TweenHandle, 0, 1, Delegate);

	InitAI();
}

void ACAICharacter::Tick(float DeltaTime)
{
	if (bDead) return;
	
	Super::Tick(DeltaTime);
	
	TickAI(DeltaTime);
}

void ACAICharacter::OnHitTaken(const FAttackData& AttackData)
{
	Super::OnHitTaken(AttackData);

	bDead = true;

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


	if (!GetMesh()->IsVisible())
	{
		GetMesh()->SetVisibility(true);
	}
	if (Value >= 1.f)
	{
		WeaponMesh->SetVisibility(true);

		BeginAILogic();

		UTweenSubsystem* TweenSubsystem = GetGameInstance()->GetSubsystem<UTweenSubsystem>();
		FTweenHandle TweenHandle;
		FTweenDynamicDelegate Delegate;
		Delegate.BindDynamic(this, &ThisClass::TweenDrone);

		TweenSubsystem->AddTween(TweenHandle, 0, 1, Delegate);
	}
	else if (bDead && Value <= 0.f)
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACAICharacter::TweenDrone(float Value)
{
	/* If the weapon mesh is visible our bot has finished spawning and we can take the drone up */
	const float ActualValue = WeaponMesh->IsVisible() ? 1 - Value : Value;
	const float HeightOffset = FMath::Lerp(DroneStartHeightOffset, DroneEndHeightOffset, ActualValue);
	FVector LocationVector = FVector::ZeroVector;
	LocationVector.Z = HeightOffset;

	DroneMesh->SetRelativeLocation(LocationVector);

	if (ActualValue >= 1.f && !WeaponMesh->IsVisible())
	{
		ReadyActor();
		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
	}
	else if (ActualValue <= 0.1f && WeaponMesh->IsVisible())
	{
		DroneMesh->SetVisibility(false);
	}
}

void ACAICharacter::ReadyActor()
{
	Super::ReadyActor();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}

void ACAICharacter::TickAI_Implementation(float DeltaSeconds)
{
	
}

void ACAICharacter::InitAI_Implementation()
{
	
}

void ACAICharacter::BeginAILogic_Implementation()
{

}
