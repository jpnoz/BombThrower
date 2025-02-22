// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BTGameStateBase.h"

#include "GameFramework/PlayerState.h"
#include "Interactable/InteractableBase.h"
#include "Component/Objective/BTDefendObjectiveComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABTGameStateBase::ABTGameStateBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

void ABTGameStateBase::BeginPlay()
{
	// Gather All Defend Objectives in world
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABTDefendObjectiveBase::StaticClass(), AllDefendObjectives);

	// Subscribe to each objective's Destroyed Event
	for (AActor* Objective : AllDefendObjectives)
	{
		ABTDefendObjectiveBase* DefendObjective = Cast<ABTDefendObjectiveBase>(Objective);

		DefendObjective->DefendObjectiveComponent->OnDefendObjectiveDestroyed.AddDynamic(this, &ABTGameStateBase::OnDefendObjectiveDestroyed);
	}
}

void ABTGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetAllPlayerActors();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractableBase::StaticClass(), AllInteractables);
}

void ABTGameStateBase::GetAllPlayerActors()
{
	// Clear Current Array of AllPlayers before regathering
	AllPlayers.Empty();

	TArray<APlayerState*> AllPlayerStates = PlayerArray;
	for (APlayerState* CurrentPlayerState : AllPlayerStates)
	{
		AllPlayers.Add(CurrentPlayerState->GetPawn());
	}
}

void ABTGameStateBase::OnDefendObjectiveDestroyed_Implementation(AActor* DestroyedObjective)
{
}