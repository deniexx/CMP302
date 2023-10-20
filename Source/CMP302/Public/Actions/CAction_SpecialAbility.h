// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_SpecialAbility.generated.h"

/**
 * 
 */
UCLASS()
class CMP302_API UCAction_SpecialAbility : public UCAction
{
	GENERATED_BODY()

public:

	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;
};
