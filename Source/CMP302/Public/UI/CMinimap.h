// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMinimap.generated.h"

class UOverlay;
class UCMinimapWidget;
class UImage;
/**
 * 
 */
UCLASS()
class CMP302_API UCMinimap : public UUserWidget
{
	GENERATED_BODY()

public:

	void AddEnemyToMinimap(AActor* EnemyToAdd);

	void RemoveEnemyFromMinimap(const AActor* EnemyToRemove);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:

	UPROPERTY(meta = (BindWidget))
	UOverlay* ParentOverlay;

	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCMinimapWidget> MinimapWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<UCMinimapWidget*> MinimapIcons;

	UPROPERTY(EditDefaultsOnly)
	float PositionScale = 32.f;
};
