// Coded by: Denis Dimitrov for CMP302


#include "UI/CDashUserWidget.h"

#include "ActorComponents/CCharacterMovementComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

void UCDashUserWidget::BindMovementComponent(UCCharacterMovementComponent* InMovementComponent)
{
	MovementComponent = InMovementComponent;
}

void UCDashUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (MovementComponent)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(this, UIParameters, "DashTimerProgress", MovementComponent->GetDashTimerProgress());
	}
}
