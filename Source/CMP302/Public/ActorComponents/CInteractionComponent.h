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

	void PrimaryInteract();

protected:
	
	void FindBestInteractable();

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel;

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCWorldUserWidget> DefaultWidgetClass;
	
	UPROPERTY()
	UCWorldUserWidget* DefaultWidgetInstance;
};
