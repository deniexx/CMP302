// Coded by: Denis Dimitrov for CMP302


#include "Actors/CVoidActor.h"

#include "Components/PrimitiveComponent.h"

// Sets default values
ACVoidActor::ACVoidActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	VoidMode = EVoidMode::None;
	LiveModeColor = FLinearColor(0.2f, 0.21, 0.08f, 1.f);
	GhostModeColor = FLinearColor(0.08f, 0.2f, 0.22f);
}

// Called when the game starts or when spawned
void ACVoidActor::BeginPlay()
{
	Super::BeginPlay();

	LiveModeAlphaReduce = 0.f;
	GhostModeAlphaReduce = 0.9f;
	UpdateColourOnStateSwitched(VoidMode);
	OnStateSwitched(VoidMode);
}

void ACVoidActor::SwitchVoidState()
{
	switch (VoidMode)
	{
	case EVoidMode::None: // If we were live or none, go to ghost
	case EVoidMode::Live:
		VoidMode = EVoidMode::Ghost;
		break;
	case EVoidMode::Ghost: // If we were ghost, go to live
		VoidMode = EVoidMode::Live;
		break;
	}

	UpdateColourOnStateSwitched(VoidMode);
	OnStateSwitched(VoidMode);
}

void ACVoidActor::UpdateColourOnStateSwitched(EVoidMode NewVoidMode)
{
	FLinearColor ColorToSet;
	float AlphaReduceAmount = 0.f;
	ECollisionEnabled::Type CollisionType;
	switch (VoidMode)
	{
	case EVoidMode::Live:
		ColorToSet = LiveModeColor;
		CollisionType = ECollisionEnabled::QueryAndPhysics;
		AlphaReduceAmount = LiveModeAlphaReduce;
		break;
	case EVoidMode::Ghost:
		ColorToSet = GhostModeColor;
		CollisionType = ECollisionEnabled::NoCollision;
		AlphaReduceAmount = GhostModeAlphaReduce;
		break;
	}

	for (UMaterialInstanceDynamic* Material : DynamicMaterials)
	{
		Material->SetVectorParameterValue("Color", ColorToSet);
		Material->SetScalarParameterValue("AlphaReduceAmount", AlphaReduceAmount);
	}

	for (UPrimitiveComponent* Component : VoidModeComponents)
	{
		Component->SetCollisionEnabled(CollisionType);
	}
}

void ACVoidActor::OnStateSwitched_Implementation(EVoidMode NewVoidMode)
{
	// @NOTE: This must be implemented in blueprints
}
