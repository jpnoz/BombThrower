// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BTEnemyBase.h"

#include "Component/BTHealthComponent.h"

#include "Game/BTGameStateBase.h"
#include "Interactable/BTBombBase.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"

// Sets default values
ABTEnemyBase::ABTEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BombDetectionRadius = 1500.0f;
	BombDetectionRate = 0.5f;
	InertiaDecayRate = 1.5f;
	NewMovementAngleThreshold = 5.0f;

	PlayerDetectionRadius = 3500.0f;
	PlayerDetectionRate = 0.2f;
	AimAdjustmentRate = 9.0f;
	BaseSpawnParameters = FSpawnerParameters();
	BaseSpawnParameters.BaseSpawnImpulse = FVector::Zero();
	BaseSpawnParameters.bRandomizeSpawnImpulse = true;

	ThrowForce = 2000.0f;
}

// Called when the game starts or when spawned
void ABTEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	GameState = Cast<ABTGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	LastMovementVector = FVector::Zero();
	CurrentSpawnParameters = BaseSpawnParameters;
}

// Called every frame
void ABTEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), BombDetectionRadius, 32, FColor::Yellow, false, 0.1f);

	AddMovementInput(CurrentMovementVector, 1.0f);
	LastMovementVector -= LastMovementVector * InertiaDecayRate * DeltaTime;

	if (LastMovementVector.SquaredLength() < 0.1f)
	{
		LastMovementVector = FVector::Zero();
	}

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (500 * CurrentMovementVector), FColor::Red, false, 0.1f);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (500 * LastMovementVector), FColor::Blue, false, 0.1f);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + CurrentAimVector, FColor::Green, false, 0.1f);
}

void ABTEnemyBase::UpdateMovementVector()
{
	if (LastMovementVector.Equals(FVector::Zero()))
	{
		LastMovementVector = CurrentMovementVector;
	}

	CurrentMovementVector = CalculateMovementVector();

	// If angle between CurrentMovement and LastMovement > Threshold
	// Add Last to Current
	float AngleBetween = UKismetMathLibrary::DegAcos(FVector::DotProduct(CurrentMovementVector, LastMovementVector));
	if (!CurrentMovementVector.Equals(FVector::Zero()) && AngleBetween >= NewMovementAngleThreshold)
	{
		CurrentMovementVector += LastMovementVector;
	}
}

FVector ABTEnemyBase::CalculateMovementVector()
{
	FVector NewMovementVector = FVector::Zero();
	FVector LateralMovementVector = FVector::Zero();
	TArray<FVector> BombToEnemyPositions = FindBombPositions();
	int32 NumBombThreats = BombToEnemyPositions.Num();

	for (int i = 0; i < NumBombThreats; i++)
	{
		NewMovementVector += BombToEnemyPositions[i];
	}

	if (NumBombThreats >= 2 && NewMovementVector.SquaredLength() < 1)
	{
		LateralMovementVector = FVector::CrossProduct(NewMovementVector, GetActorUpVector()) * (1 - NewMovementVector.Length());
	}

	NewMovementVector = (NewMovementVector + LateralMovementVector) * FVector(1.0f, 1.0f, 0.0f);
	NewMovementVector.Normalize();


	return NewMovementVector;
}

TArray<FVector> ABTEnemyBase::FindBombPositions()
{
	TArray<FVector> BombToEnemyVectors;
	
	// Sphere Trace for all actors in detection radius
	// Instead of GetAllActorsWithTag() to prevent crash when too many bombs exist at once
	for (int i = 0; i < GameState->AllInteractables.Num(); i++)
	{
		AActor* ActorToCheck = GameState->AllInteractables[i];

		if (!ActorToCheck->IsA(ABTBombBase::StaticClass()))
		{
			continue;
		}

		FVector DistanceToActor = GetActorLocation() - ActorToCheck->GetActorLocation();

		if (DistanceToActor.SquaredLength() > (BombDetectionRadius * BombDetectionRadius))
		{
			continue;
		}

		BombToEnemyVectors.Add(DistanceToActor);
	}

	return BombToEnemyVectors;
}

void ABTEnemyBase::UpdateAimVector()
{
	CurrentAimVector = CalculateAimVector();
}

FVector ABTEnemyBase::CalculateAimVector()
{
	TArray<APlayerState*> AllPlayers = GameState->PlayerArray;

	FVector ClosestEnemyToPlayerVector = FVector::Zero();
	for (int i = 0; i < AllPlayers.Num(); i++)
	{
		APlayerState* PlayerToCheck = AllPlayers[i];

		FVector DistanceToPlayer = PlayerToCheck->GetPawn()->GetActorLocation() - GetActorLocation();
		
		if (DistanceToPlayer.SquaredLength() > (PlayerDetectionRadius * PlayerDetectionRadius))
		{
			continue;
		}

		if (i == 0 || DistanceToPlayer.SquaredLength() < ClosestEnemyToPlayerVector.SquaredLength())
		{
			ClosestEnemyToPlayerVector = DistanceToPlayer;
		}
	}

	FVector NewAimVector = ClosestEnemyToPlayerVector;
	if (ClosestEnemyToPlayerVector.SquaredLength() > ThrowForce * ThrowForce)
	{
		NewAimVector.Normalize();
		NewAimVector *= ThrowForce;
	}

	NewAimVector += FVector(0, 0, ThrowForce);
	return NewAimVector;
}
