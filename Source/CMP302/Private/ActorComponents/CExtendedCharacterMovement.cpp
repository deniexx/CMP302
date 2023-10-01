// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CExtendedCharacterMovement.h"

void UCExtendedCharacterMovement::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	OnMovementChanged.Broadcast(PreviousMovementMode, PreviousCustomMode);
}
