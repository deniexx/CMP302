// Coded by: Denis Dimitrov for CMP302


#include "Actions/CAction_Minimap.h"

#include "Blueprint/UserWidget.h"
#include "Character/CAICharacter.h"
#include "UI/CMinimap.h"
#include "Character/CCommonCharacter.h"
#include "Components/SphereComponent.h"

void UCAction_Minimap::OnActionAdded_Implementation(AActor* InInstigator)
{
	Super::OnActionAdded_Implementation(InInstigator);

	Character = Cast<ACCommonCharacter>(InInstigator);
	PlayerController = Character->GetController<APlayerController>();

	MinimapInstance = CreateWidget<UCMinimap>(PlayerController, MinimapClass);
	MinimapInstance->AddToViewport();

	SphereCollision = Cast<USphereComponent>(Character->AddComponentByClass(USphereComponent::StaticClass(), true, FTransform(), false));
	SphereCollision->SetSphereRadius(SphereSize);
	SphereCollision->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	SphereCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SphereCollision->SetCollisionObjectType(ECC_GameTraceChannel2);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapStarted);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnded);
}

void UCAction_Minimap::OnActionRemoved_Implementation(AActor* InInstigator)
{
	Super::OnActionRemoved_Implementation(InInstigator);

	MinimapInstance->RemoveFromParent();

	SphereCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnOverlapStarted);
	SphereCollision->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::OnOverlapEnded);
	SphereCollision->DestroyComponent();
}

void UCAction_Minimap::OnOverlapStarted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACAICharacter::StaticClass()))
		MinimapInstance->AddEnemyToMinimap(OtherActor);
}

void UCAction_Minimap::OnOverlapEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (OtherActor->IsA(ACAICharacter::StaticClass()))
		MinimapInstance->RemoveEnemyFromMinimap(OtherActor);
}
