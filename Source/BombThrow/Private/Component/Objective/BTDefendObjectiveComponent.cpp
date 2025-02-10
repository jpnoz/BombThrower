 // Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Objective/BTDefendObjectiveComponent.h"
#include "Moveable/MoveableBase.h"

// Sets default values for this component's properties
UBTDefendObjectiveComponent::UBTDefendObjectiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBTDefendObjectiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// Gather All Moveable Pieces that make up the Objective Mesh
	TArray<USceneComponent*> AllChildren;
	GetOwner()->GetRootComponent()->GetChildrenComponents(true, AllChildren);

	for (USceneComponent* Child : AllChildren)
	{
		UStaticMeshComponent* Moveable = Cast<UStaticMeshComponent>(Child);
		if (Moveable)
		{
			ObjectiveMoveables.Add(Moveable);
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Children: %d, Found: %d"), AllChildren.Num(), ObjectiveMoveables.Num()));
	}
}

// Called every frame
void UBTDefendObjectiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UBTDefendObjectiveComponent::EnableObjectiveMoveables()
{
	OnDefendObjectiveDestroyed.Broadcast();

	for (UStaticMeshComponent* MoveableMesh : ObjectiveMoveables)
	{
		MoveableMesh->SetSimulatePhysics(true);
	}
}