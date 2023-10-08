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
		return;

	LastFrameNumberWeTicked = GFrameCounter;
	const int32 Size = Tweens.Num() - 1;
	if (Size < 0) return;

	for (int i = Size; i >= 0; --i)
	{
		if (Tweens[i].bActive)
		{
			Tweens[i].TweenValue += DeltaTime * Tweens[i].TweenSpeed;
			//Tweens[i].TweenValue = Tweens[i].TweeningFunction(Tweens[i].TweenValue); // @TODO: Figure out why the function is not working properly

			float Value = FMath::Lerp(Tweens[i].TweenStart, Tweens[i].TweenEnd, Tweens[i].TweenValue);
			Value = FMath::Clamp(Value, 0, 1.f);
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
			if (Tweens[i].TweenValue >= 1.f)
			{
				Tweens[i].Handle->Invalidate();
				Tweens.RemoveAt(i);
			}
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
	return ETickableTickType::Conditional;
}

UWorld* UTweenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* GameInstance = GetGameInstance())
		return GameInstance->GetWorld();

	return nullptr;
}

bool UTweenSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UTweenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTweenSubsystem, STATGROUP_Tickables);;
}

void UTweenSubsystem::AddTween(FTweenHandle& InOutHandle, float Start, float End, FTweenDelegate const& Delegate, float Speed,
                                            ETweenFunction TweenFunction, bool Active)
{
	InOutHandle.Index = Index;
	FTween& Tween = Tweens.Add_GetRef(FTween(Start, End));
	Tween.bActive = Active;
	Tween.TweenSpeed = Speed;
	Tween.Delegate = Delegate;
	Tween.TweeningFunction = GetTweenFunction(TweenFunction);
	Tween.Index = Index;
	Tween.Handle = &InOutHandle;
	++Index;
}

void UTweenSubsystem::AddTween(FTweenHandle& InOutHandle, float Start, float End, FTweenDynamicDelegate const& Delegate, float Speed,
	ETweenFunction TweenFunction, bool Active)
{
	InOutHandle.Index = Index;
	FTween& Tween = Tweens.Add_GetRef(FTween(Start, End));
	Tween.bActive = Active;
	Tween.TweenSpeed = Speed;
	Tween.DynamicDelegate = Delegate;
	Tween.TweeningFunction = GetTweenFunction(TweenFunction);
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

void UTweenSubsystem::StopTween(FTweenHandle& TweenHandle)
{
	if (!TweenHandle.IsValid()) return;
	
	for (const FTween& Tween : Tweens)
	{
		if (Tween.Index == TweenHandle.GetIndex())
		{
			TweenHandle.Invalidate();
			Tweens.Remove(Tween);
			break;
		}
	}
}

TFunction<float (float Value)> UTweenSubsystem::GetTweenFunction(ETweenFunction Function)
{
	switch (Function)
	{
		case ETweenFunction::None:
			return [](float Value)
			{
				return Value;
			};
		case ETweenFunction::EaseIn:
			return [](float Value)
			{
				return 1 - (FMath::Cos((Value * UE_PI) / 2));
			};
		case ETweenFunction::EaseOut:
			return [](float Value)
			{
				return FMath::Sin((Value * UE_PI) / 2);
			};
		case ETweenFunction::EaseInOut:
			return [](float Value)
			{
				return -(FMath::Cos(UE_PI * Value) - 1) / 2;
			};
		case ETweenFunction::EaseInQuad:
			return [](float Value)
			{
				return Value * Value;
			};
		case ETweenFunction::EaseOutQuad:
			return [](float Value)
			{
				return 1 - (1 - Value) * (1 - Value);
			};
		case ETweenFunction::EaseInOutQuad:
			return [](float Value)
			{
				return Value < 0.5f ? 2 * Value * Value : 1 - FMath::Pow(-2 * Value + 2, 2) / 2;
			};
		case ETweenFunction::EaseInCubic:
			return [](float Value)
			{
				return Value * Value * Value;
			};
		case ETweenFunction::EaseOutCubic:
			return [](float Value)
			{
				return 1 - FMath::Pow(1 - Value, 3);
			};
		case ETweenFunction::EaseInOutCubic:
			return [](float Value)
			{
				return Value < 0.5f ? 4 * Value * Value * Value : 1 - FMath::Pow(-2 * Value + 2, 3) / 2;
			};
		case ETweenFunction::EaseInQuint:
			return [](float Value)
			{
				return Value * Value * Value * Value;
			};
		case ETweenFunction::EaseOutQuint:
			return [](float Value)
			{
				return 1 - FMath::Pow(1 - Value, 5);
			};
		case ETweenFunction::EaseInOutQuint:
			return [](float Value)
			{
				return Value < 0.5f ? 16 * Value * Value * Value * Value : 1 - FMath::Pow(-2 * Value + 2, 5) / 2;
			};
	}

	return nullptr;
}
