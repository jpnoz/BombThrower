// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/BTInteractableSpawnerBase.h"

#include "Game/BTGameStateBase.h"
#include "Interactable/InteractableBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABTInteractableSpawnerBase::ABTInteractableSpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnTime = 5.0f;
	SpawnRadius = 0.0f;
	BaseSpawnImpulse = FVector::Zero();
	bRandomizeSpawnImpulse = false;
}

// Called every frame
void ABTInteractableSpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnedInteractables = FindSpawnedInteractables();
}

TArray<AActor*> ABTInteractableSpawnerBase::FindSpawnedInteractables()
{
	ABTGameStateBase* GameState = Cast<ABTGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	TArray<AActor*> OwnedInteractables;

	for (int i = 0; i < GameState->AllInteractables.Num(); i++)
	{
		AActor* TargetInteractable = GameState->AllInteractables[i];
		if (TargetInteractable->Owner == this)
		{
			OwnedInteractables.Add(TargetInteractable);
		}
	}

	return OwnedInteractables;
}

void ABTInteractableSpawnerBase::SpawnInteractable()
{
	ABTGameStateBase* GameState = Cast<ABTGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));

	if (GameState->AllInteractables.Num() >= GameState->MaxInteractables || SpawnedInteractables.Num() >= SpawnLimit)
	{
		return;
	}

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	if (SpawnRadius > 0.0f)
	{
		FVector RandomSpawn = UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(0, SpawnRadius);
		SpawnLocation += RandomSpawn;
	}

	AInteractableBase* SpawnedInteractable = GetWorld()->SpawnActor<AInteractableBase>(InteractableToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	//SpawnedInteractables.Add(SpawnedInteractable);
	
	UStaticMeshComponent* InteractableMesh = Cast<UStaticMeshComponent>(SpawnedInteractable->GetRootComponent());
	InteractableMesh->SetSimulatePhysics(true);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	LaunchInteractable(InteractableMesh);
}

void ABTInteractableSpawnerBase::LaunchInteractable(UStaticMeshComponent* InteractableMesh)
{
	FVector SpawnImpulse = BaseSpawnImpulse;
	if (bRandomizeSpawnImpulse)
	{
		float RandomImpulseX = UKismetMathLibrary::RandomFloatInRange(MinSpawnImpulseVariance.X, MaxSpawnImpulseVariance.X);
		float RandomImpulseY = UKismetMathLibrary::RandomFloatInRange(MinSpawnImpulseVariance.Y, MaxSpawnImpulseVariance.Y);
		float RandomImpulseZ = UKismetMathLibrary::RandomFloatInRange(MinSpawnImpulseVariance.Z, MaxSpawnImpulseVariance.Z);

		FVector RandomImpulse = FVector(RandomImpulseX, RandomImpulseY, RandomImpulseZ);
		SpawnImpulse += RandomImpulse;
	}

	InteractableMesh->AddImpulse(SpawnImpulse * InteractableMesh->GetMass());
}

