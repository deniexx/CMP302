// Coded by: Denis Dimitrov for CMP302

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_Minimap.generated.h"

class USphereComponent;
class UCMinimap;
class ACCommonCharacter;
/**
 * 
 */
UCLASS()
class CMP302_API UCAction_Minimap : public UCAction
{
	GENERATED_BODY()

public:
	
	virtual void OnActionAdded_Implementation(AActor* InInstigator) override;
	
	virtual void OnActionRemoved_Implementation(AActor* InInstigator) override;

protected:

	/** The minimap widget class to display */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSubclassOf<UCMinimap> MinimapClass;

	/** The sphere size for collision checks */
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	float SphereSize;

	/** The sphere collision component to check for overlaps to display on the minimap */
	UPROPERTY()
	USphereComponent* SphereCollision;
	
private:

	UFUNCTION()
	void OnOverlapStarted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

	UPROPERTY()
	ACCommonCharacter* Character;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UCMinimap* MinimapInstance;
};
