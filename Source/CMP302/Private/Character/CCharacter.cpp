// Coded by: Denis Dimitrov for CMP302

#include "CMP302/Public/Character/CCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/CCharacterMovementComponent.h"
#include "ActorComponents/CCombatComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACCharacter::ACCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

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
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	CombatComponent = CreateDefaultSubobject<UCCombatComponent>(TEXT("CombatComponent"));
}

// Called when the game starts or when spawned
void ACCharacter::BeginPlay()
{
	Super::BeginPlay();

	ExtendedMovementComponent = GetCharacterMovement<UCCharacterMovementComponent>();
	CombatComponent->OnHitTaken.AddDynamic(this, &ThisClass::OnHitTaken);
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	CombatComponent->Init();
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
	}
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

void ACCharacter::OnHitTaken()
{
	GEngine->AddOnScreenDebugMessage(0, 10, FColor::Green, "HitTaken");
}
