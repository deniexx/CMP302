// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CGameplayUISubsystem.generated.h"

class UUserWidget;

USTRUCT()
struct FGameplayUIData
{
	GENERATED_BODY()

	UPROPERTY()
	ECInputMode InputMode = ECInputMode::None;

	UPROPERTY()
	bool bShowMouseCursor = false;

	UPROPERTY()
	bool bFocusWidget = false;

	UPROPERTY()
	bool bPaused = false;

	UPROPERTY()
	UUserWidget* Widget;
};

/**
 * 
 */
UCLASS()
class CMP302_API UCGameplayUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "GameplayUI")
	UUserWidget* PushWidget(TSubclassOf<UUserWidget> WidgetClass, ECInputMode InputMode = ECInputMode::None, bool bShowMouseCursor = false, bool bFocusWidget = false, bool bPause = false);

	UFUNCTION(BlueprintCallable, Category = "GameplayUI")
	bool PopWidget();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayUI")
	bool IsWidgetInStack();
	
	void BindPlayerController(APlayerController* InPlayerController);
	
protected:

	UPROPERTY()
	TArray<FGameplayUIData> WidgetStack;

private:

	UPROPERTY()
	APlayerController* PlayerController;

	void SetInputMode(const FGameplayUIData& Data);
};
