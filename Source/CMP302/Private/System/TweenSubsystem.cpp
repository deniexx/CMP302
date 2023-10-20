// Coded by: Denis Dimitrov for CMP302


#include "System/TweenSubsystem.h"

void UTweenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Tweens.Empty();
}

void UTweenSubsystem::Tick(float DeltaTime)
{
	if (LastFrameNumberWeTicked == GFrameCounter)
	{
		return;
	}
	
	LastFrameNumberWeTicked = GFrameCounter;
	const int32 Size = Tweens.Num() - 1;
	if (Size < 0) return;

	for (int i = Size; i >= 0; --i)
	{
		if (Tweens[i].bActive)
		{
			Tweens[i].TweenValue = Tweens[i].TweenValue + (DeltaTime * Tweens[i].TweenSpeed);

			const float Value = FMath::Clamp(Tweens[i].TweenValue, Tweens[i].TweenStart, Tweens[i].TweenEnd);
			
			/* Call events bound here */
			if (Tweens[i].Delegate.IsBound())
			{
				Tweens[i].Delegate.Execute(Value);
			}
			if (Tweens[i].DynamicDelegate.IsBound())
			{
				Tweens[i].DynamicDelegate.Execute(Value);
			}

			/* If we have reached the end of the tween, remove the object */
			if (Tweens[i].TweenValue >= Tweens[i].TweenEnd)
			{
				Tweens[i].bActive = false;
				Tweens[i].bRemove = true;
			}
		}
		else if (Tweens[i].bRemove)
		{
			Tweens[i].Handle->Invalidate();
			Tweens.RemoveAt(i);
		}
	}
}

void UTweenSubsystem::Deinitialize()
{
	Super::Deinitialize();

	Tweens.Empty();
}

ETickableTickType UTweenSubsystem::GetTickableTickType() const
{
	return ETickableTickType::Always;
}

UWorld* UTweenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* GameInstance = GetGameInstance())
		return GameInstance->GetWorld();

	return nullptr;
}

bool UTweenSubsystem::IsTickableWhenPaused() const
{
	return true;
}

TStatId UTweenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTweenSubsystem, STATGROUP_Tickables);;
}

void UTweenSubsystem::AddTween(FTweenHandle& InOutHandle, float Start, float End, FTweenDelegate const& Delegate, float Speed, bool bActive)
{
	InOutHandle.Index = Index;
	FTween& Tween = Tweens.Add_GetRef(FTween(Start, End));
	Tween.bActive = bActive;
	Tween.TweenSpeed = Speed;
	Tween.Delegate = Delegate;
	Tween.Index = Index;
	Tween.Handle = &InOutHandle;
	++Index;
}

void UTweenSubsystem::AddTween(FTweenHandle& InOutHandle, float Start, float End, FTweenDynamicDelegate const& Delegate, float Speed, bool bActive)
{
	InOutHandle.Index = Index;
	FTween& Tween = Tweens.Add_GetRef(FTween(Start, End));
	Tween.bActive = bActive;
	Tween.TweenSpeed = Speed;
	Tween.DynamicDelegate = Delegate;
	Tween.Index = Index;
	Tween.Handle = &InOutHandle;
	++Index;
}

void UTweenSubsystem::SetTweenActive(const FTweenHandle& TweenHandle, bool bIsActive)
{
	if (!TweenHandle.IsValid()) return;
	
	for (FTween& Tween : Tweens)
	{
		if (Tween.Index == TweenHandle.GetIndex())
		{
			Tween.bActive = bIsActive;
			break;
		}
	}
}

void UTweenSubsystem::StopTween(FTweenHandle& TweenHandle, bool bRemove)
{
	if (!TweenHandle.IsValid()) return;

	for (int i = Tweens.Num() - 1; i >= 0; --i)
	{
		if (Tweens[i] == TweenHandle)
		{
			Tweens[i].bActive = false;

			if (bRemove)
			{
				Tweens[i].bRemove = true;
			}

			return;
		}
	}
}
