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

	ACAICharacter();

	virtual void ReadyActor() override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* DroneMesh;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* SpawnSound;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float DroneStartHeightOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float DroneEndHeightOffset;

	UFUNCTION()
	void TweenDrone(float Value);
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnHitTaken(const FAttackData& AttackData) override;

	UFUNCTION(BlueprintNativeEvent)
	void InitAI();

	UFUNCTION(BlueprintNativeEvent)
	void BeginAILogic();

	virtual void TweenAppearance(float Value) override;

	UFUNCTION(BlueprintNativeEvent)
	void TickAI(float DeltaSeconds);
};
