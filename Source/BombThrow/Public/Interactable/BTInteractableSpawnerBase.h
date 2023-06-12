// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BTInteractableSpawnerBase.generated.h"

class AInteractableBase;

UCLASS()
class BOMBTHROW_API ABTInteractableSpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABTInteractableSpawnerBase();

protected:

	UFUNCTION(BlueprintCallable, Category = "BombThrower|Spawning")
	void SpawnInteractable();
	TArray<AActor*> FindSpawnedInteractables();
	void LaunchInteractable(UStaticMeshComponent* InteractableMesh);

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AInteractableBase> InteractableToSpawn;
	TArray<AActor*> SpawnedInteractables;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning|Parameters")
	float SpawnTime;

	UPROPERTY(EditAnywhere, Category = "Spawning|Parameters")
	float SpawnRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning|Parameters")
	int32 SpawnLimit;

	UPROPERTY(EditAnywhere, Category = "Spawning|Parameters")
	FVector BaseSpawnImpulse;

	UPROPERTY(EditAnywhere, Category = "Spawning|Randomization")
	bool bRandomizeSpawnImpulse;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bRandomizeSpawnImpulse"), Category = "Spawning|Randomization")
	FVector MinSpawnImpulseVariance;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bRandomizeSpawnImpulse"), Category = "Spawning|Randomization")
	FVector MaxSpawnImpulseVariance;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
