// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BTEnemyBase.h"

#include "Component/BTHealthComponent.h"

#include "Game/BTGameStateBase.h"
#include "Interactable/BTBombBase.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "DrawDebugHelpers.h"

// Sets default values
ABTEnemyBase::ABTEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BombDetectionRadius = 1500.0f;
	WallDetectionRadius = 500.0f;
	WallAngleThreshold = 15.0f;
	PlayerDetectionRadius = 9000.0f;
	PlayerAimingRadius = 3500.0f;

	BombMovementWeight = 1.0f;
	WallMovementWeight = 3.0f;

	PlayerDetectionRate = 0.2f;
	MovementAdjustmentRate = 0.5f;
	AimAdjustmentRate = 9.0f;

	MaxWalkSpeedThreshold = 0.2f;
	InertiaDecayRate = 1.5f;
	NewMovementAngleThreshold = 5.0f;

	BaseSpawnParameters = FSpawnerParameters();
	BaseSpawnParameters.bCanSpawn = true;
	BaseSpawnParameters.SpawnTime = 0.2f;
	BaseSpawnParameters.SpawnRadius = 0.0f;
	BaseSpawnParameters.SpawnLimit = 3;
	BaseSpawnParameters.BaseSpawnImpulse = FVector::Zero();
	BaseSpawnParameters.bRandomizeSpawnImpulse = true;
	BaseSpawnParameters.MinSpawnImpulseVariance = FVector(-100.0f, -100.0f, 0.0f);
	BaseSpawnParameters.MaxSpawnImpulseVariance = FVector(100.0f, 100.0f, 0.0f);

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

	//DrawDebugSphere(GetWorld(), GetActorLocation(), PlayerAimingRadius, 32, FColor::Yellow, false, 0.1f);

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Speed: %f"), CurrentMovementVector.SquaredLength()));
	}*/

	AddMovementInput(CurrentMovementVector, CurrentMovementVector.SquaredLength() / MaxWalkSpeedThreshold * MaxWalkSpeedThreshold);
	LastMovementVector -= LastMovementVector * InertiaDecayRate * DeltaTime;

	if (LastMovementVector.SquaredLength() < 0.1f)
	{
		LastMovementVector = FVector::Zero();
	}

	//DrawDebugLine(GetWorld(), GetActorLocation() + FVector(0.0f, 0.0f, 50.0f), GetActorLocation() + (500 * CurrentMovementVector), FColor::Red, false, 0.1f);
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

	// If CurrentMovement is still 0
	// Move toward player
	if (CurrentMovementVector.Equals(FVector::Zero()))
	{
		FVector TargetPlayerPosition = GetActorLocation() + FindClosestPlayerDirection(PlayerDetectionRadius);
		//DrawDebugSphere(GetWorld(), TargetPlayerPosition, 150.0f, 32, FColor::Orange, false, 2.0f);
		MoveToPlayer(TargetPlayerPosition);
	}
}

FVector ABTEnemyBase::CalculateMovementVector()
{
	FVector NewMovementVector = FVector::Zero();
	FVector LateralMovementVector = FVector::Zero();

	FVector BombMovementVector = CalculateBombAvoidance() * BombMovementWeight;
	FVector WallMovementVector = CalculateWallAvoidance() * WallMovementWeight;

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("%f / %f"), BombMovementVector.SquaredLength(), WallMovementVector.SquaredLength()));
	}*/

	NewMovementVector = BombMovementVector + WallMovementVector;
	//NewMovementVector.Normalize();

	return NewMovementVector;
}

void ABTEnemyBase::UpdateAimVector()
{
	CurrentSpawnParameters.bCanSpawn = true;

	CurrentAimVector = CalculateAimVector();
	if (CurrentAimVector.Equals(FVector::Zero()))
	{
		CurrentSpawnParameters.bCanSpawn = false;
	}

	CurrentSpawnParameters.BaseSpawnImpulse = CurrentAimVector;

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + CurrentAimVector, FColor::Cyan, false, 1.0f);
}

FVector ABTEnemyBase::CalculateAimVector()
{
	FVector NewAimVector = FindClosestPlayerDirection(PlayerAimingRadius);

	// Limit Throwing Vector to enemy's max throw force
	if (NewAimVector.SquaredLength() > ThrowForce * ThrowForce)
	{
		NewAimVector.Normalize();
		NewAimVector *= ThrowForce;
	}

	if (NewAimVector.SquaredLength() > PlayerAimingRadius * PlayerAimingRadius)
	{
		NewAimVector = FVector::Zero();
	}
	
	if (!NewAimVector.Equals(FVector::Zero()))
	{
		NewAimVector += FVector(0, 0, ThrowForce);
	}
	
	return NewAimVector;
}

FVector ABTEnemyBase::CalculateBombAvoidance()
{
	FVector BombMovementVector = FVector::Zero();
	FVector LateralMovementVector = FVector::Zero();
	TArray<FVector> BombToEnemyPositions = FindBombPositions();
	int32 NumBombThreats = BombToEnemyPositions.Num();

	for (int i = 0; i < NumBombThreats; i++)
	{
		// Adjust each vector from bomb to enemy inversely proportional to distance
		FVector CurrentBombVector = BombToEnemyPositions[i];
		float VectorAdjustmentFactor = 1.0f - (CurrentBombVector.SquaredLength() / (BombDetectionRadius * BombDetectionRadius));

		CurrentBombVector.Normalize();
		BombMovementVector += CurrentBombVector * VectorAdjustmentFactor;
	}

	if (NumBombThreats >= 2 && BombMovementVector.SquaredLength() < 1.0f)
	{
		LateralMovementVector = FVector::CrossProduct(BombMovementVector, GetActorUpVector()) * BombMovementVector.Length();
	}

	BombMovementVector = (BombMovementVector + LateralMovementVector) * FVector(1.0f, 1.0f, 0.0f);
	return BombMovementVector;
}

FVector ABTEnemyBase::CalculateWallAvoidance()
{
	FVector WallMovementVector = FVector::Zero();
	FVector LateralMovementVector = FVector::Zero();
	TArray<FHitResult> HitObstacles;
	int32 NumObstacles = 0;

	if (bSphereTrace(GetActorLocation(), GetActorLocation() + (100 * GetActorForwardVector()), HitObstacles))
	{
		NumObstacles = HitObstacles.Num();
	}

	for (int i = 0; i < NumObstacles; i++)
	{
		FHitResult CurrentObstacle = HitObstacles[i];

		// Ignore collisions with bombs
		if (CurrentObstacle.GetActor()->IsA(ABTBombBase::StaticClass()))
		{
			continue;
		}

		FVector HitLocation = CurrentObstacle.ImpactPoint;
		FVector EnemyToObstacleVector = HitLocation - GetActorLocation();
		
		FVector HitDirection = CurrentObstacle.ImpactNormal * FVector(1.0f, 1.0f, 0.0f);
		
		// Angle of normal from horizontal must be within angle range
		if (HitDirection.SquaredLength() < UKismetMathLibrary::DegCos(WallAngleThreshold))
		{
			continue;
		}

		float VectorAdjustmentFactor = 1.0f - (EnemyToObstacleVector.SquaredLength() / (WallDetectionRadius * WallDetectionRadius));
		HitDirection.Normalize();
		WallMovementVector += HitDirection * VectorAdjustmentFactor;

		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + EnemyToObstacleVector, FColor::Orange, false, 2.0f);
		DrawDebugLine(GetWorld(), HitLocation, HitLocation + (HitDirection * 100), FColor::Emerald, false, 2.0f);
	}

	return WallMovementVector;
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

FVector ABTEnemyBase::FindClosestPlayerDirection(float MaxRange)
{
	TArray<APlayerState*> AllPlayers = GameState->PlayerArray;

	FVector ClosestEnemyToPlayerVector = FVector::Zero();
	for (int i = 0; i < AllPlayers.Num(); i++)
	{
		APlayerState* PlayerToCheck = AllPlayers[i];

		FVector DistanceToPlayer = PlayerToCheck->GetPawn()->GetActorLocation() - GetActorLocation();

		if (DistanceToPlayer.SquaredLength() > (MaxRange * MaxRange))
		{
			continue;
		}

		//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + DistanceToPlayer, FColor::Cyan, false, 1.0f);

		if (i == 0 || DistanceToPlayer.SquaredLength() < ClosestEnemyToPlayerVector.SquaredLength())
		{
			ClosestEnemyToPlayerVector = DistanceToPlayer;
		}
	}

	return ClosestEnemyToPlayerVector;
}

void ABTEnemyBase::MoveToPlayer_Implementation(FVector PlayerPosition)
{
	//DrawDebugSphere(GetWorld(), PlayerPosition, 150.0f, 32, FColor::Orange, false, 2.0f);
}

bool ABTEnemyBase::bSphereTrace(FVector StartLocation, FVector EndLocation, TArray<FHitResult>& TraceResults)
{
	FCollisionShape TraceSphere = FCollisionShape::MakeSphere(WallDetectionRadius);
	bool bSurfaceDetected = GetWorld()->SweepMultiByChannel(TraceResults, StartLocation, EndLocation, GetActorRotation().Quaternion(), ECC_Visibility, TraceSphere);
	return bSurfaceDetected;
}