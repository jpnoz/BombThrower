// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BTDebugComponent.h"

#include "Interactable/InteractableBase.h"
#include "Component/BTInteractionComponent.h"

// Sets default values for this component's properties
UBTDebugComponent::UBTDebugComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bHoldSpawnedInteractable = false;
}

void UBTDebugComponent::SpawnObject()
{
	// Don't spawn object if one already held
	if (bHoldSpawnedInteractable && CurrentHeldInteractable)
	{
		UE_LOG(LogTemp, Warning, TEXT("New object cannot be created."));
	}

	FVector SpawnLocation = GetOwner()->GetActorLocation() + (GetOwner()->GetActorUpVector() * 100);
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	AInteractableBase* SpawnedInteractable = GetWorld()->SpawnActor<AInteractableBase>(InteractableToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

	if (bHoldSpawnedInteractable)
	{
		CurrentHeldInteractable = SpawnedInteractable;
		GetOwner()->FindComponentByClass<UBTInteractionComponent>()->AttachHeldInteractable();
	}
	else
	{
		SpawnedInteractable->SetActorLocation(SpawnLocation + GetOwner()->GetActorForwardVector() * 200);
	}
}

