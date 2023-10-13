// Coded by: Denis Dimitrov for CMP302


#include "Character/CPlayerState.h"

#include "CMP302/CMP302GameMode.h"
#include "System/CSaveGame.h"

ACPlayerState::ACPlayerState()
{
	Currency = 0;
}

int32 ACPlayerState::GetCurrency()
{
	return Currency;
}

void ACPlayerState::AddCurrency(int32 Amount)
{
	Currency += Amount;
	SaveCurrency();
}

bool ACPlayerState::RemoveCurrency(int32 Amount)
{
	if (Currency - Amount < 0)
	{
		return false;
	}

	Currency -= Amount;
	SaveCurrency();
	return true;
}

void ACPlayerState::SaveCurrency() const
{
	ACMP302GameMode* GameMode = GetWorld()->GetAuthGameMode<ACMP302GameMode>();
	UCSaveGame* SaveGame = GameMode->GetSaveGame();
	SaveGame->Currency = Currency;
	
	GameMode->WriteSaveGame();
}
