// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMinimapWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class CMP302_API UCMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UImage* Icon;

	UPROPERTY()
	AActor* AttachedActor;
};
