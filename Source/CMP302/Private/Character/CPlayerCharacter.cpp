// Coded by: Denis Dimitrov for CMP302


#include "Character/CPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/CActionComponent.h"
#include "ActorComponents/CCombatStatusComponent.h"
#include "ActorComponents/CExtendedCharacterMovement.h"
#include "ActorComponents/CInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "../CMP302GameMode.h"
#include "System/CSaveGame.h"

ACPlayerCharacter::ACPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCExtendedCharacterMovement>(CharacterMovementComponentName))
{
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10, 0 , 60.f)); // Again default values used by Epic in the template
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	InteractionComponent = CreateDefaultSubobject<UCInteractionComponent>(TEXT("InteractionComponent"));
	
	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->SetupAttachment(FirstPersonCameraComponent);
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -165.f));
	
	bResetTransform = false;
	bDead = false;
	RoomIndex = 0;
}

void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ACMP302GameMode* GameMode = GetWorld()->GetAuthGameMode<ACMP302GameMode>();
	GameMode->BindToOnHitDelegateForPlayer(this);
	
	const UCSaveGame* SaveGame = GameMode->GetSaveGame();
	const FString LevelName = GetWorld()->GetMapName();

	if (SaveGame->SpawnTransforms.Find(LevelName))
	{
		SpawnTransform = SaveGame->SpawnTransforms[LevelName];
	}
	else
	{
		SpawnTransform = GetActorTransform();
	}
	
	bResetTransform = true;
	ReadyActor();
}



void ACPlayerCharacter::OnHitTaken(const FAttackData& AttackData)
{
	Super::OnHitTaken(AttackData);

	OnPlayerHit.Broadcast(RoomIndex);
	bResetTransform = true;
	ReadyActor();
}

void ACPlayerCharacter::ReadyActor()
{
	Super::ReadyActor();

	ActionComponent->ActiveGameplayTags.AddTag(InputBlockTag);

	if (bResetTransform)
	{
		bResetTransform = false;
		SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::ResetPhysics);
	}
}

void ACPlayerCharacter::TweenAppearance(float Value)
{
	Super::TweenAppearance(Value);

	if (Value >= 1.f)
		ActionComponent->ActiveGameplayTags.RemoveTag(InputBlockTag);
}

void ACPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ACPlayerCharacter::BeginTraversal);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::EndTraversal);

		// Move and Look actions
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Look);

		// Fire Projectile
		EnhancedInputComponent->BindAction(FireProjectileInputAction, ETriggerEvent::Started, this, &ACPlayerCharacter::FireProjectile);

		// Switch Status
		EnhancedInputComponent->BindAction(SwitchAttackStatusInputAction, ETriggerEvent::Started, this, &ACPlayerCharacter::SwitchAttackStatus);

		// Dash
		EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Started, this, &ACPlayerCharacter::BeginDash);
		EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::EndDash);

		// Slash
		EnhancedInputComponent->BindAction(SlashInputAction, ETriggerEvent::Started, this, &ACPlayerCharacter::SlashAttack);

		// Slide
		EnhancedInputComponent->BindAction(SlideInputAction, ETriggerEvent::Started, this, &ACPlayerCharacter::BeginSlide);
		EnhancedInputComponent->BindAction(SlideInputAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::EndSlide);

		// Overload
		EnhancedInputComponent->BindAction(OverloadInputAction, ETriggerEvent::Started, this, &ACPlayerCharacter::OverloadCharacter);

		// Interact
		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Started, this, &ACPlayerCharacter::Interact);
	}
}

void ACPlayerCharacter::SetSpawnTransform(const FTransform& InSpawnTransform)
{
	SpawnTransform = InSpawnTransform;

	ACMP302GameMode* GameMode = GetWorld()->GetAuthGameMode<ACMP302GameMode>();
	UCSaveGame* SaveGame = GameMode->GetSaveGame();

	const FString LevelName = GetWorld()->GetMapName();
	
	SaveGame->SpawnTransforms.Emplace(LevelName, SpawnTransform);
	
	GameMode->WriteSaveGame();
}

void ACPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (ActionComponent->ActiveGameplayTags.HasAnyExact(InputBlockingTags))
		return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (ActionComponent->ActiveGameplayTags.HasAnyExact(InputBlockingTags))
		return;
	
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		
		// @NOTE: Here we could add a check for inverting if the player has selected inverted controls
		AddControllerPitchInput(LookVector.Y);
	}
}

void ACPlayerCharacter::SwitchAttackStatus(const FInputActionValue& Value)
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

void ACPlayerCharacter::FireProjectile(const FInputActionValue& Value)
{
	if (ensureAlways(ActionComponent))
		ActionComponent->StartActionByTag(this, FireProjectileActionTag);
}

void ACPlayerCharacter::SlashAttack(const FInputActionValue& Value)
{
	if (ensureAlways(ActionComponent))
		ActionComponent->StartActionByTag(this, SwordSlashActionTag);
}

void ACPlayerCharacter::BeginDash(const FInputActionValue& Value)
{
	if (ensureAlways(ActionComponent))
		ActionComponent->StartActionByTag(this, DashActionTag);
}

void ACPlayerCharacter::EndDash(const FInputActionValue& Value)
{
	if (ensureAlways(ActionComponent))
		ActionComponent->StopActionByTag(this, DashActionTag);
}

void ACPlayerCharacter::BeginTraversal(const FInputActionValue& Value)
{
	if (ensureAlways(ActionComponent))
		ActionComponent->StartActionByTag(this, TraversalActionTag);
}

void ACPlayerCharacter::EndTraversal(const FInputActionValue& Value)
{
	if (ensureAlways(ActionComponent))
		ActionComponent->StopActionByTag(this, TraversalActionTag);
}

void ACPlayerCharacter::BeginSlide(const FInputActionValue& Value)
{
	if (ensureAlways(ActionComponent))
		ActionComponent->StartActionByTag(this, SlideActionTag);
}

void ACPlayerCharacter::EndSlide(const FInputActionValue& Value)
{
	if (ensureAlways(ActionComponent))
		ActionComponent->StopActionByTag(this, SlideActionTag);
}

void ACPlayerCharacter::OverloadCharacter(const FInputActionValue& Value)
{
	if (ensureAlways(ActionComponent))
		ActionComponent->StartActionByTag(this, OverloadActionTag);
}

void ACPlayerCharacter::Interact(const FInputActionValue& Value)
{
	if (ensureAlways(InteractionComponent))
		InteractionComponent->PrimaryInteract();
}
