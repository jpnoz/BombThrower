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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float BombDetectionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float BombDetectionRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Movement", meta = (UIMin = 0.1, UIMax = 10.0))
	float InertiaDecayRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Movement", meta = (UIMin = 1.0, UIMax = 359.0))
	float NewMovementAngleThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float PlayerDetectionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float PlayerDetectionRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Perception")
	float AimAdjustmentRate;

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

	UFUNCTION(BlueprintCallable, Category = "Enemy|Movement")
	void UpdateMovementVector();
	FVector CalculateMovementVector();
	TArray<FVector> FindBombPositions();

	UFUNCTION(BlueprintCallable, Category = "Enemy|Aim")
	void UpdateAimVector();
	FVector CalculateAimVector();

	class ABTGameStateBase* GameState;
	FVector LastMovementVector;
	FVector CurrentMovementVector;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Aim")
	FVector CurrentAimVector;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
