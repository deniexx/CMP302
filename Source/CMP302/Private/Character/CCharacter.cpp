// Coded by: Denis Dimitrov for CMP302

#include "CMP302/Public/Character/CCharacter.h"

#include "AIController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/CCharacterMovementComponent.h"
#include "ActorComponents/CCombatComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "System/CGameplayFunctionLibrary.h"

// Sets default values
ACCharacter::ACCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	// Default values used by Epic in the template
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10, 0 , 60.f)); // Again default values used by Epic in the template
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Set up first person mesh by template example
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -165.f));

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(Mesh1P, "hand_r");
	SwordMesh->bCastDynamicShadow = false;
	SwordMesh->CastShadow = false;

	ShurikenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShurikenMesh"));
	ShurikenMesh->SetupAttachment(Mesh1P, "hand_l");
	ShurikenMesh->bCastDynamicShadow = false;
	ShurikenMesh->CastShadow = false;

	CombatComponent = CreateDefaultSubobject<UCCombatComponent>(TEXT("CombatComponent"));

	bDead = false;
	AppearanceAlpha = 0.f;
}

// Called when the game starts or when spawned
void ACCharacter::BeginPlay()
{
	Super::BeginPlay();

	ExtendedMovementComponent = GetCharacterMovement<UCCharacterMovementComponent>();
	
	CombatComponent->OnHitTaken.AddDynamic(this, &ACCharacter::OnHitTaken);
	CombatComponent->Init(ShurikenMesh);
	
	USkeletalMeshComponent* MeshComponent = Mesh1P->GetSkeletalMeshAsset() ? Mesh1P : GetMesh();
	const int32 NumMaterials = MeshComponent->GetNumMaterials();
	for (int i = 0; i < NumMaterials; ++i)
	{
		MeshDynamicMaterials.Add(MeshComponent->CreateDynamicMaterialInstance(i, MeshComponent->GetMaterial(i)));
	}

	SwordMesh->CreateDynamicMaterialInstance(0, SwordMesh->GetMaterial(0));
	SwordMesh->CreateDynamicMaterialInstance(1, SwordMesh->GetMaterial(1));

	SwordMesh->IgnoreActorWhenMoving(this, true);
	SwordMesh->OnComponentBeginOverlap.AddDynamic(this, &ACCharacter::OnSwordHit);
	SwitchSwordCollision(false);
	
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);
	}
	else
	{
		MeshDynamicMaterials[1]->SetVectorParameterValue("LogoLayer0_Color", UCGameplayFunctionLibrary::GetColorFromAttackStatus(CombatComponent->GetAttackStatusType()));
		MeshDynamicMaterials[1]->SetVectorParameterValue("LogoLayer1_Color", UCGameplayFunctionLibrary::GetColorFromAttackStatus(CombatComponent->GetAttackStatusType()));
		MeshDynamicMaterials[1]->SetVectorParameterValue("LogoLayer2_Color", UCGameplayFunctionLibrary::GetColorFromAttackStatus(CombatComponent->GetAttackStatusType()));
	}
}

void ACCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float AlphaTarget = bDead ? 0.f : 1.f;

	if (!FMath::IsNearlyEqual(AppearanceAlpha, AlphaTarget))
	{
		AppearanceAlpha += bDead ? -DeltaSeconds : DeltaSeconds;
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
	else if (bDead && FMath::IsNearlyZero(AlphaTarget))
	{
		Destroy();
	}
	else if (FMath::IsNearlyEqual(AppearanceAlpha, 1))
	{
		if (APlayerController* PlayerController = GetController<APlayerController>())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
				PlayerController->SetIgnoreLookInput(false);
				PlayerController->SetIgnoreMoveInput(false);
			}
		}
	}
}

// Called to bind functionality to input
void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		// @TODO: Implement a custom character movement that can handle wall running and vaulting
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Move and Look actions
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACCharacter::Look);

		// Fire Projectile
		EnhancedInputComponent->BindAction(FireProjectileAction, ETriggerEvent::Started, this, &ACCharacter::FireProjectile);

		// Switch Status
		EnhancedInputComponent->BindAction(SwitchAttackStatusAction, ETriggerEvent::Started, this, &ACCharacter::SwitchAttackStatus);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ACCharacter::BeginDash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ACCharacter::EndDash);

		// Slash
		EnhancedInputComponent->BindAction(SlashAction, ETriggerEvent::Started, this, &ACCharacter::SlashAttack);
	}
}

void ACCharacter::SwitchSwordCollision(bool bEnabled) const
{
	SwordMesh->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void ACCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		
		// @NOTE: Here we could add a check for inverting if the player has selected inverted controls
		AddControllerPitchInput(LookVector.Y);
	}
}

void ACCharacter::FireProjectile(const FInputActionValue& Value)
{
	if (ensureAlways(CombatComponent))
		CombatComponent->FireProjectile();
}

void ACCharacter::SwitchAttackStatus(const FInputActionValue& Value)
{
	const FVector2D InputVector = Value.Get<FVector2D>();

	/**
	 * Mapping for Input Vector to Attack Status:
	 * X > 0 : Red
	 * X < 0 : Green
	 * Y > 0 : Blue
	 */
	if (!ensureAlways(CombatComponent)) return;
	
	if (InputVector.X > 0)
		CombatComponent->UpdateAttackStatusType(EAttackStatusType::Red);
	else if (InputVector.X < 0)
		CombatComponent->UpdateAttackStatusType(EAttackStatusType::Green);
	else if (InputVector.Y > 0)
		CombatComponent->UpdateAttackStatusType(EAttackStatusType::Blue);
		
}

void ACCharacter::BeginDash(const FInputActionValue& Value)
{
	ExtendedMovementComponent->BeginDash();
}

void ACCharacter::EndDash(const FInputActionValue& Value)
{
	ExtendedMovementComponent->EndDash();
}

void ACCharacter::SlashAttack(const FInputActionValue& Value)
{
	CombatComponent->SlashAttack();
}

void ACCharacter::OnHitTaken(const FAttackData& AttackData)
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
		return;

	
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

	OnHitTaken_BP(AttackData);
}

void ACCharacter::OnHitTaken_BP_Implementation(const FAttackData& AttackData)
{
	
}

void ACCharacter::OnSwordHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FAttackData AttackData;
	AttackData.Instigator = this;
	AttackData.AttackStatusType = CombatComponent->GetAttackStatusType();
	AttackData.ImpactStrength = 30000.f;
	UCGameplayFunctionLibrary::TryRegisterHit(AttackData, OtherActor);
}
