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
}

// Called when the game starts or when spawned
void ABTInteractableSpawnerBase::BeginPlay()
{
	Super::BeginPlay();

	SetSpawnParameters(InitialSpawnParameters);
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
	if (!CurrentSpawnParameters.bCanSpawn)
	{
		return;
	}

	ABTGameStateBase* GameState = Cast<ABTGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));

	if (GameState->AllInteractables.Num() >= GameState->MaxInteractables || SpawnedInteractables.Num() >= CurrentSpawnParameters.SpawnLimit)
	{
		return;
	}

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	if (CurrentSpawnParameters.SpawnRadius > 0.0f)
	{
		FVector RandomSpawn = UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(0, CurrentSpawnParameters.SpawnRadius);
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
	FVector SpawnImpulse = CurrentSpawnParameters.BaseSpawnImpulse;
	if (CurrentSpawnParameters.bRandomizeSpawnImpulse)
	{
		float RandomImpulseX = UKismetMathLibrary::RandomFloatInRange(CurrentSpawnParameters.MinSpawnImpulseVariance.X, CurrentSpawnParameters.MaxSpawnImpulseVariance.X);
		float RandomImpulseY = UKismetMathLibrary::RandomFloatInRange(CurrentSpawnParameters.MinSpawnImpulseVariance.Y, CurrentSpawnParameters.MaxSpawnImpulseVariance.Y);
		float RandomImpulseZ = UKismetMathLibrary::RandomFloatInRange(CurrentSpawnParameters.MinSpawnImpulseVariance.Z, CurrentSpawnParameters.MaxSpawnImpulseVariance.Z);

		FVector RandomImpulse = FVector(RandomImpulseX, RandomImpulseY, RandomImpulseZ);
		SpawnImpulse += RandomImpulse;
	}

	InteractableMesh->AddImpulse(SpawnImpulse * InteractableMesh->GetMass());
}

void ABTInteractableSpawnerBase::SetSpawnParameters(FSpawnerParameters NewSpawnParameters)
{
	CurrentSpawnParameters = NewSpawnParameters;
	OnParametersUpdated();
}

void ABTInteractableSpawnerBase::OnParametersUpdated_Implementation()
{
}

