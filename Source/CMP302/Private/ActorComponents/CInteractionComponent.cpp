// Coded by: Denis Dimitrov for CMP302


#include "ActorComponents/CInteractionComponent.h"

#include "CLogChannels.h"
#include "UI/CWorldUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/CInteractionInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("sc.DebugDrawInteraction"),
	false,
	TEXT("Draw debug sphere for line interaction."),
	ECVF_Cheat);

// Sets default values for this component's properties
UCInteractionComponent::UCInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	TraceCollisionChannel = ECC_WorldDynamic;
}

// Called every frame
void UCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void UCInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		UE_LOG(LogCMP, Log, TEXT("No Focus Actor to interact"));
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ICInteractionInterface::Execute_Interact(FocusedActor, MyPawn);
}

void UCInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TraceCollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	const FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);
	
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	
	for (FHitResult Hit: Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (bDebugDraw)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f, 0, 2.0f);
			}
			
			if (HitActor->Implements<UCInteractionInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
			DefaultWidgetInstance = CreateWidget<UCWorldUserWidget>(GetWorld(), DefaultWidgetClass);

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
				DefaultWidgetInstance->AddToViewport();
		}
	}
	else
	{
		if (DefaultWidgetInstance)
			DefaultWidgetInstance->RemoveFromParent();
	}
	
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0.0f, 2.0f);
	}
}
