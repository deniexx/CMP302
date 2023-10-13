// Coded by: Denis Dimitrov for CMP302


#include "Character/CPlayerState.h"

#include "CMP302/CMP302GameMode.h"
#include "System/CSaveGame.h"

ACPlayerState::ACPlayerState()
{
	Currency = 0;
}

void ACPlayerState::BeginPlay()
{
	Super::BeginPlay();

	const ACMP302GameMode* GameMode = GetWorld()->GetAuthGameMode<ACMP302GameMode>();
	if (const UCSaveGame* SaveGame = GameMode->GetSaveGame())
		AddCurrency(SaveGame->Currency);
}

int32 ACPlayerState::GetCurrency()
{
	return Currency;
}

void ACPlayerState::AddCurrency(int32 Amount)
{
	const int32 OldCurrency = Currency;
	Currency += Amount;
	SaveCurrency();
	
	OnCurrencyUpdated.Broadcast(Currency, OldCurrency);
}

bool ACPlayerState::RemoveCurrency(int32 Amount)
{
	if (Currency - Amount < 0)
	{
		return false;
	}

	const int32 OldCurrency = Currency;
	Currency -= Amount;
	SaveCurrency();

	OnCurrencyUpdated.Broadcast(Currency, OldCurrency);
	return true;
}

void ACPlayerState::SaveCurrency() const
{
	ACMP302GameMode* GameMode = GetWorld()->GetAuthGameMode<ACMP302GameMode>();
	UCSaveGame* SaveGame = GameMode->GetSaveGame();
	SaveGame->Currency = Currency;
	
	GameMode->WriteSaveGame();
}
