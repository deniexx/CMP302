// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CExtendedCharacterMovement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMovementModeChanged, EMovementMode, PreviousMovementMode, uint8, PreviousCustomMovementMode);

/**
 * 
 */
UCLASS()
class CMP302_API UCExtendedCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnMovementModeChanged OnMovementChanged;

protected:

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
};
