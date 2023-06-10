// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DamageableBase.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyMovement", meta = (UIMin = 0.1, UIMax = 10.0))
	float InertiaDecayRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyMovement", meta = (UIMin = 1.0, UIMax = 359.0))
	float NewMovementAngleThreshold;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "EnemyMovement")
	void UpdateMovementVector();
	FVector CalculateMovementVector();
	TArray<FVector> FindBombPositions();

	FVector LastMovementVector;
	FVector CurrentMovementVector;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
