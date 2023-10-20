// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CMP302_API UCInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Performs an interaction if a target has been focused */
	void PrimaryInteract();

protected:

	/** Finds the best interactable to focus */
	void FindBestInteractable();

	/** The length of the trace for the interaction checks */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceDistance;

	/** The radius of the interaction sphere collision check */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceRadius;

	/** The collision channels used to check for interaction */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel;

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCWorldUserWidget> DefaultWidgetClass;
	
	UPROPERTY()
	UCWorldUserWidget* DefaultWidgetInstance;
};
