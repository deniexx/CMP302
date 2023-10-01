// Coded by: Denis Dimitrov for CMP302


#include "Character/CCommonCharacter.h"

#include "ActorComponents/CActionComponent.h"
#include "ActorComponents/CCombatStatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
ACCommonCharacter::ACCommonCharacter(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default values used by Epic in the template
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	CombatComponent = CreateDefaultSubobject<UCCombatStatusComponent>(TEXT("CombatComponent"));
	ActionComponent = CreateDefaultSubobject<UCActionComponent>(TEXT("ActionComponent"));

	bDead = false;
	AppearanceAlpha = 0.f;
}

// Called when the game starts or when spawned
void ACCommonCharacter::BeginPlay()
{
	Super::BeginPlay();

	const int32 NumMaterials = GetMesh()->GetNumMaterials();
	for (int i = 0; i < NumMaterials; ++i)
	{
		MeshDynamicMaterials.Add(GetMesh()->CreateDynamicMaterialInstance(i, GetMesh()->GetMaterial(i)));
	}
	
	CombatComponent->Init();
	CombatComponent->OnHitTaken.AddDynamic(this, &ThisClass::OnHitTaken);
}

void ACCommonCharacter::OnHitTaken(const FAttackData& AttackData)
{
	OnHitTaken_BP(AttackData);
}

void ACCommonCharacter::OnHitTaken_BP_Implementation(const FAttackData& AttackData)
{
	
}

void ACCommonCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	OnCharacterLanded.Broadcast(Hit);
}

// Called every frame
void ACCommonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float AlphaTarget = bDead ? 0.f : 1.f;

	if (!FMath::IsNearlyEqual(AppearanceAlpha, AlphaTarget))
	{
		AppearanceAlpha += bDead ? -DeltaTime : DeltaTime;
		AppearanceAlpha = FMath::Clamp(AppearanceAlpha, 0, 1);

		if (UMaterialParameterCollection* ParamCollection = CombatComponent ? CombatComponent->GetPlayerMaterialParameters() : nullptr)
		{
			UKismetMaterialLibrary::SetScalarParameterValue(this, ParamCollection, "Appearance", AppearanceAlpha);
		}
		
		for (UMaterialInstanceDynamic* Material : MeshDynamicMaterials)
		{
			Material->SetScalarParameterValue("Appearance", AppearanceAlpha);
		}
	}
}

void ACCommonCharacter::ReadyActor()
{
	bDead = false;
	AppearanceAlpha = 0.f;
}

bool ACCommonCharacter::IsDead() const
{
	return bDead;
}
