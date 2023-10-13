// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "CMP302/CCustomTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CGameplayUISubsystem.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class CMP302_API UCGameplayUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GameplayUI")
	UUserWidget* PushWidget(TSubclassOf<UUserWidget> WidgetClass, bool bAddToViewport = false);

	UFUNCTION(BlueprintCallable, Category = "GameplayUI")
	UUserWidget* PopWidget(bool bRemoveFromViewport = false);

	UFUNCTION(BlueprintCallable, Category = "GameplayUI")
	void SetInputMode(ECInputMode InputMode, bool bInShowMouseCursor = false, UUserWidget* WidgetToFocus = nullptr);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayUI")
	bool IsWidgetInStack();
	
	void BindPlayerController(APlayerController* InPlayerController);
	
protected:

	UPROPERTY()
	TArray<UUserWidget*> WidgetStack;

private:

	UPROPERTY()
	APlayerController* PlayerController;
};
