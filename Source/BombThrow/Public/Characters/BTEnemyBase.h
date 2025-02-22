// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DamageableBase.h"
#include "Interactable/BTInteractableSpawnerBase.h"
#include "GameFramework/Character.h"
#include "BTEnemyBase.generated.h"

UCLASS()
class BOMBTHROW_API ABTEnemyBase : public ADamageableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABTEnemyBase();

	UPROPERTY(BlueprintReadWrite, Category = "Enemy|Movement")
	bool bCanAddMovementInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float BombDetectionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float WallDetectionRadius;

	// Maximum deviation from vertical that a surface can be to be considered a wall
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception", meta = (UIMin = 0.0, UIMax = 90.0))
	float WallAngleThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float PlayerDetectionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float PlayerAimingRadius;

	// Multiplier for bomb avoidance vectors
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float BombMovementWeight;
	
	// Multiplier for obstacle avoidance vectors
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float WallMovementWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float PlayerDetectionRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float MovementAdjustmentRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float AimAdjustmentRate;

	// Minimum magnitude a movement vector must be to move at full speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Movement")
	float MaxWalkSpeedThreshold;

	// How quickly the last movement vector fades when a new one is calculated,
	// in percent / second
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Movement", meta = (UIMin = 0.1, UIMax = 10.0))
	float InertiaDecayRate;

	// How large of a variance a new movement vector needs to have (in degrees)
	// from last movement vector to be considered "new"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Movement", meta = (UIMin = 1.0, UIMax = 359.0))
	float NewMovementAngleThreshold;

	// NOTE: Editing Base Spawn Impulse has no effect
	UPROPERTY(EditAnywhere, Category = "Gameplay|Spawning")
	FSpawnerParameters BaseSpawnParameters;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay|Spawning")
	FSpawnerParameters CurrentSpawnParameters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Spawning")
	float ThrowForce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FVector> GetAllActorLocations(TArray<AActor*> Actors);

	UFUNCTION(BlueprintCallable, Category = "Enemy|Movement")
	void UpdateMovementVector();
	FVector CalculateMovementVector();
	FVector DetermineTargetLocation();

	UFUNCTION(BlueprintCallable, Category = "Enemy|Aim")
	void UpdateAimVector();
	FVector CalculateAimVector();

	FVector CalculateBombAvoidance();
	FVector CalculateWallAvoidance();

	TArray<FVector> FindBombPositions();
	FVector FindClosestTargetDirection(TArray<FVector> TargetPositions, float MaxRange = INFINITY);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|Movement")
	void OnAIMovementRequired(FVector TargetPosition);
	void OnAIMovementRequired_Implementation(FVector TargetPosition);

	bool bSphereTrace(FVector Location, FVector EndLocation, TArray<FHitResult>& TraceResults);

	class ABTGameStateBase* GameState;
	FVector LastMovementVector;
	FVector CurrentMovementVector;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Aim")
	FVector CurrentAimVector;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
