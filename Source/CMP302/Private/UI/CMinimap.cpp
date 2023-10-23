// Coded by: Denis Dimitrov for CMP302


#include "UI/CMinimap.h"

#include "CLogChannels.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "UI/CMinimapWidget.h"

void UCMinimap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const APawn* OwningPawn = GetOwningPlayerPawn();

	if (!OwningPawn) return;

	for (UCMinimapWidget* Widget : MinimapIcons)
	{
		if (!Widget->AttachedActor)
		{
			Widget->RemoveFromParent();
			continue;
		}
		
		const FVector ForwardVector = OwningPawn->GetActorForwardVector();
		FVector WidgetDirection = Widget->AttachedActor->GetActorLocation() - OwningPawn->GetActorLocation();
		const float NormalizedDistance = WidgetDirection.Length() / 2048.f;
		WidgetDirection.Normalize();

		const FVector Cross = FVector::CrossProduct(ForwardVector, WidgetDirection);
		float Angle =  FMath::Acos(FVector::DotProduct(ForwardVector, WidgetDirection));

		Angle *= Cross.Z > 0 ? 1.f : -1;
		const float ScreenOffset = PositionScale * NormalizedDistance;
		FVector2D ScreenPosition;
		ScreenPosition.Y = -(ScreenOffset * FMath::Cos(Angle));
		ScreenPosition.X = ScreenOffset * FMath::Sin(Angle);
		Widget->SetRenderTranslation(ScreenPosition);
	}
}

void UCMinimap::AddEnemyToMinimap(AActor* EnemyToAdd)
{
	UCMinimapWidget* Widget = CreateWidget<UCMinimapWidget>(GetOwningPlayer(), MinimapWidgetClass);
	Widget->AttachedActor = EnemyToAdd;
	UOverlaySlot* WidgetSlot = ParentOverlay->AddChildToOverlay(Widget);
	WidgetSlot->SetHorizontalAlignment(HAlign_Center);
	WidgetSlot->SetVerticalAlignment(VAlign_Center);
	MinimapIcons.Add(Widget);
}

void UCMinimap::RemoveEnemyFromMinimap(const AActor* EnemyToRemove)
{
	for (UCMinimapWidget* Widget : MinimapIcons)
	{
		if (Widget->AttachedActor == EnemyToRemove)
		{
			Widget->RemoveFromParent();
		}
	}
}
