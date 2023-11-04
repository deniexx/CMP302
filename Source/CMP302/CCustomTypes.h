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

UENUM(BlueprintType)
enum class EVoidMode : uint8
{
	None UMETA(DisplayName = "None"), // This is just the default
	Live UMETA(DisplayName = "Live"), // This means that the mesh has collision and is currently "enabled"
	Ghost UMETA(DisplayName = "Ghost") // This mean s that the mesh has no collision and is currently "disabled"
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

	/** The impact strength to be applied when getting hit */
	UPROPERTY(BlueprintReadWrite, Category = "Attack Data")
	float ImpactStrength = 300.f;
};

UENUM(BlueprintType)
enum class ECInputMode : uint8
{
	None UMETA(DisplayName = "None"),
	UIOnly UMETA(DisplayName = "UIOnly"),
	GameOnly UMETA(DisplayName = "GameOnly"),
};
