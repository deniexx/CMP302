#pragma once

#include "CoreMinimal.h"

#include "CCustomTypes.generated.h"

UENUM(BlueprintType)
enum class EAttackStatusType : uint8
{
	None UMETA(DisplayName = "None"),
	Green UMETA(DisplayName = "Green"),
	Red UMETA(DisplayName = "Red"),
	Blue UMETA(DisplayName = "Blue"),
	Enemy UMETA(DisplayName = "Enemy"), // This attack is used by the enemies, it will always hit the player and avoid enemies
	White UMETA(DisplayName = "White") // This attack always hits, it can be used by the cheats too
};

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

public:

	/** Attack Status of the current attack */
	UPROPERTY(BlueprintReadWrite, Category = "Attack Data")
	EAttackStatusType AttackStatusType = EAttackStatusType::None;

	/** The actor that instigated this attack */
	UPROPERTY(BlueprintReadWrite, Category = "Attack Data")
	AActor* Instigator = nullptr;
};