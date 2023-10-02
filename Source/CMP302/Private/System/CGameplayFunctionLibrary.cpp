// Coded by: Denis Dimitrov for CMP302


#include "System/CGameplayFunctionLibrary.h"

#include "ActorComponents/CCombatStatusComponent.h"
#include "Character/CCommonCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/CProjectile.h"
#include "System/CStatusReportSubsystem.h"

FLinearColor UCGameplayFunctionLibrary::GetColorFromAttackStatus(EAttackStatusType AttackStatus)
{
	FLinearColor ToReturn;
	
	switch (AttackStatus)
	{
		case EAttackStatusType::Green:
			ToReturn = FLinearColor::Green;
			break;
		case EAttackStatusType::Red:
			ToReturn = FLinearColor::Red;
			break;
		case EAttackStatusType::Blue:
			ToReturn = FLinearColor::Blue;
			break;
		case EAttackStatusType::Enemy:
			ToReturn = FLinearColor::Red;
			break;
		case EAttackStatusType::White:
			ToReturn = FLinearColor::White;
			break;
		default:
			ToReturn = FLinearColor::Black;
			break;;
	}

	return ToReturn;
}

bool UCGameplayFunctionLibrary::TryRegisterHit(const FAttackData& AttackData, AActor* TargetActor)
{
	if (const UCCombatStatusComponent* CombatComponent = TargetActor ? UCCombatStatusComponent::GetCombatStatusComponent(TargetActor) : nullptr)
	{
		return CombatComponent->TryRegisterHit(AttackData);
	}

	return false;
}

ACProjectile* UCGameplayFunctionLibrary::SpawnProjectile(const UObject* WorldContextObject, TSubclassOf<ACProjectile> ProjectileClass,
                                                         ACCommonCharacter* Character)
{
	if (!ensureAlways(ProjectileClass) || !Character) return nullptr;
	
	FCollisionShape Shape;
	Shape.SetSphere(20.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		
	FVector TraceStart = Character->GetPawnViewLocation();
	FVector TraceEnd = TraceStart + (Character->GetControlRotation().Vector() * 5000);

	FHitResult HitResult;
	if (WorldContextObject->GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
	{
		TraceEnd = HitResult.ImpactPoint;
	}
		
	FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - TraceStart).Rotator();
	TraceStart = (Character->GetActorForwardVector() * 100.f) + TraceStart;
	TraceStart.Z -= 10;
		
	USkeletalMeshComponent* Mesh = Character->GetMesh();
	FVector SpawnLocation = Mesh->GetSocketLocation("FireProjectileSocket");
		
	FTransform SpawnTransform = FTransform(ProjectileRotation, SpawnLocation);
	ACProjectile* Projectile = WorldContextObject->GetWorld()->SpawnActorDeferred<ACProjectile>(ProjectileClass, SpawnTransform,
					Character, Character, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		
	Projectile->FinishSpawning(SpawnTransform);
	return Projectile;
}

UCStatusReportSubsystem* UCGameplayFunctionLibrary::GetStatusReportSubsystem(const UObject* WorldContextObject)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	return GameInstance ? GameInstance->GetSubsystem<UCStatusReportSubsystem>() : nullptr;
}

void UCGameplayFunctionLibrary::AddStatusReportMessage(const UObject* WorldContextObject, const FString& Message)
{
	if (const UCStatusReportSubsystem* StatusReportSubsystem = GetStatusReportSubsystem(WorldContextObject))
		StatusReportSubsystem->AddStatusMessage(Message);
}

void UCGameplayFunctionLibrary::UpdateStatusImage(const UObject* WorldContextObject, UTexture2D* StatusTexture)
{
	if (const UCStatusReportSubsystem* StatusReportSubsystem = GetStatusReportSubsystem(WorldContextObject))
		StatusReportSubsystem->UpdateStatusImage(StatusTexture);
}

