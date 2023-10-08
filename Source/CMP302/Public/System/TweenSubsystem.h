// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TweenSubsystem.generated.h"

DECLARE_DELEGATE_OneParam(FTweenDelegate, float CurrentValue);
DECLARE_DYNAMIC_DELEGATE_OneParam(FTweenDynamicDelegate, float, CurrentValue);

class UTweenSubsystem;

UENUM(BlueprintType)
enum class ETweenFunction : uint8
{
	None,
	EaseIn,
	EaseOut,
	EaseInOut,
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
};

USTRUCT(BlueprintType)
struct FTweenHandle
{
	GENERATED_BODY()

	friend UTweenSubsystem;
	
public:

	FTweenHandle() {}

	FTweenHandle(const int InIndex) : Index(InIndex) {}

	void Invalidate() { Index = 0; }
	bool IsValid() const { return Index != 0; }

	int GetIndex() const { return Index; }

private:
	
	uint64 Index = 0;
};

USTRUCT()
struct FTween
{
	GENERATED_BODY()

public:
	FTween()
	{
	}

	FTween(const float InTweenStart, const float InTweenEnd) : TweenStart(InTweenStart), TweenEnd(InTweenEnd)
	{
	}

	FTweenDelegate Delegate;
	FTweenDynamicDelegate DynamicDelegate;
	
	bool bActive = false;
	float TweenStart = 0;
	float TweenEnd = 0;
	float TweenValue = 0;
	float TweenSpeed = 1;
	uint64 Index = 0;

	FTweenHandle* Handle = nullptr;

	TFunction<float (float Value)> TweeningFunction;

	bool operator==(const FTween& lhs) const { return lhs.Index == Index; }
};

/**
 * 
 */
UCLASS()
class CMP302_API UTweenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	friend FTweenHandle;

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Deinitialize() override;

	virtual ETickableTickType GetTickableTickType() const override;
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual TStatId GetStatId() const override;

	/**
	 * Adds a tween to the tweening subsystem and if active is true, it begins tweening
	 * @param InOutHandle The handle of the tween
	 * @param Start The start value
	 * @param End The end value
	 * @param Delegate The delegate, holding the callback function, no callback function means you will have to get the value yourself
	 * @param Speed The speed of the tween, this will be used to multiply DeltaTime with, so if you want 4 secs, use 1 / 4 etc...
	 * @param TweenFunction The tweening function to use, None is just straight interpolation without any easing
	 * @param Active Whether the tween is active, this can be used to store it and activate later
	 * @return A tween handle, can be used to further reference tweens
	 */
	void AddTween(FTweenHandle& InOutHandle, float Start, float End, FTweenDelegate const& Delegate, float Speed = 1, ETweenFunction TweenFunction = ETweenFunction::None, bool Active = true);

	/**
	 * Adds a tween to the tweening subsystem and if active is true, it begins tweening
	 * @param InOutHandle The handle of the tween
	 * @param Start The start value
	 * @param End The end value
	 * @param Delegate The delegate, holding the callback function, no callback function means you will have to get the value yourself
	 * @param Speed The speed of the tween, this will be used to multiply DeltaTime with, so if you want 4 secs, use 1 / 4 etc...
	 * @param TweenFunction The tweening function to use, None is just straight interpolation without any easing
	 * @param Active Whether the tween is active, this can be used to store it and activate later
	 * @return A tween handle, can be used to further reference tweens
	 */
	UFUNCTION(BlueprintCallable, Category = "TweenSubsystem")
	void AddTween(FTweenHandle& InOutHandle, float Start, float End, FTweenDynamicDelegate const& Delegate, float Speed = 1, ETweenFunction TweenFunction = ETweenFunction::None, bool Active = true);

	/**
	 * Changes a tween active state
	 * @param TweenHandle The handle of the tween to use to find said tween
	 * @param bIsActive Whether the tween should be active
	 */
	UFUNCTION(BlueprintCallable, Category = "TweenSubsystem")
	void SetTweenActive(const FTweenHandle& TweenHandle, bool bIsActive);

	/**
	 * Stops a tween and removes it from the subsystem, this will also invalidate the handle
	 * @param TweenHandle The handle of the ween to use to find said tween
	 */
	UFUNCTION(BlueprintCallable, Category = "TweenSubsystem")
	void StopTween(FTweenHandle& TweenHandle);

private:
	/**
	 * Finds the appropriate tweening function to be called when we are interpolating the value
	 * @param Function Tweening function chosen
	 * @return The function to be called when we tick the tween
	 */
	TFunction<float (float Value)> GetTweenFunction(ETweenFunction Function);

	float TweenWithFunction(ETweenFunction Function, float Value);

	UPROPERTY()
	TArray<FTween> Tweens;

	uint64 Index = 1;

	uint32 LastFrameNumberWeTicked = INDEX_NONE;
};
