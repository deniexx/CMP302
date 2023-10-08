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

void ACCommonCharacter::TweenAppearance(float Value)
{
	if (UMaterialParameterCollection* ParamCollection = CombatComponent ? CombatComponent->GetPlayerMaterialParameters() : nullptr)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, ParamCollection, "Appearance", Value);
	}
		
	for (UMaterialInstanceDynamic* Material : MeshDynamicMaterials)
	{
		Material->SetScalarParameterValue("Appearance", Value);
	}
}

void ACCommonCharacter::StartTweenAppearance()
{
	FTweenHandle Handle;
	UTweenSubsystem* TweenWorldSubsystem = GetGameInstance()->GetSubsystem<UTweenSubsystem>();
	FTweenDelegate Delegate;
	Delegate.BindUObject(this, &ThisClass::TweenAppearance);
	
	const float Start = bDead ? 1 : 0;
	const float End = bDead ? 0 : 1;
	TweenWorldSubsystem->AddTween(Handle, Start, End, Delegate, 1, ETweenFunction::EaseInOutQuint);
}

void ACCommonCharacter::ReadyActor()
{
	bDead = false;

	StartTweenAppearance();
}

bool ACCommonCharacter::IsDead() const
{
	return bDead;
}
