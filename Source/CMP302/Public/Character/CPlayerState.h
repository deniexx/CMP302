// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrencyUpdated, int32, NewAmount, int32, OldAmount);

/**
 * 
 */
UCLASS()
class CMP302_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ACPlayerState();

	int32 GetCurrency();

	UFUNCTION(BlueprintCallable, Category = "Currency")
	void AddCurrency(int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Currency")
	bool RemoveCurrency(int32 Amount);

	UPROPERTY(BlueprintAssignable, Category = "Currency")
	FOnCurrencyUpdated OnCurrencyUpdated;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Currency")
	int32 Currency;

private:

	void SaveCurrency() const;
};
