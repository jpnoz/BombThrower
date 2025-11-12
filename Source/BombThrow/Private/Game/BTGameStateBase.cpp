// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BTGameStateBase.h"

#include "GameFramework/PlayerState.h"
#include "Characters/BTCharacterBase.h"
#include "Characters/BTEnemyBase.h"
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
	// Gather All Defend Objectives and Enemies in world
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABTDefendObjectiveBase::StaticClass(), AllDefendObjectives);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABTEnemyBase::StaticClass(), AllEnemies);

	// Subscribe to each Objective's Destroyed Event
	// and each Player and Enemy's Death Events
	for (AActor* Objective : AllDefendObjectives)
	{
		ABTDefendObjectiveBase* DefendObjective = Cast<ABTDefendObjectiveBase>(Objective);

		DefendObjective->DefendObjectiveComponent->OnDefendObjectiveDestroyed.AddDynamic(this, &ABTGameStateBase::OnDefendObjectiveDestroyed);
	}

	TArray<APlayerState*> AllPlayerStates = PlayerArray;
	for (APlayerState* CurrentPlayerState : AllPlayerStates)
	{
		APawn* CurrentPlayerPawn = CurrentPlayerState->GetPawn();
		AllPlayers.Add(CurrentPlayerPawn);

		ABTCharacterBase* CurrentDamageable = Cast<ABTCharacterBase>(CurrentPlayerPawn);
		CurrentDamageable->OnDamageableDeath.AddDynamic(this, &ABTGameStateBase::OnPlayerDefeated);
	}

	for (AActor* Enemy : AllEnemies)
	{
		ABTEnemyBase* CurrentEnemy = Cast<ABTEnemyBase>(Enemy);

		CurrentEnemy->OnDamageableDeath.AddDynamic(this, &ABTGameStateBase::OnEnemyDefeated);
	}
}

void ABTGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractableBase::StaticClass(), AllInteractables);
}

void ABTGameStateBase::OnPlayerDefeated_Implementation(AActor* DestroyedObjective)
{
}

void ABTGameStateBase::OnEnemyDefeated_Implementation(AActor* DestroyedObjective)
{
}

void ABTGameStateBase::OnDefendObjectiveDestroyed_Implementation(AActor* DestroyedObjective)
{
}