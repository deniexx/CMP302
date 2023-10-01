// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Character/CCommonCharacter.h"
#include "CAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class CMP302_API ACAICharacter : public ACCommonCharacter
{
	GENERATED_BODY()

public:

	virtual void ReadyActor() override;

protected:

	UPROPERTY()
	FTransform MeshTransform;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnHitTaken(const FAttackData& AttackData) override;

	UFUNCTION(BlueprintNativeEvent)
	void InitAI();

	UFUNCTION(BlueprintNativeEvent)
	void TickAI(float DeltaSeconds);
};
