// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "CDataTableRow.generated.h"

class UCAction;

USTRUCT(BlueprintType)
struct FCPurchasableDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	int32 Index;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCAction> ActionClass;
};
