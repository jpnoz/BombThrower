// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BTInteractableSpawnerBase.generated.h"

class AInteractableBase;

USTRUCT(BlueprintType)
struct FSpawnerParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BaseSpawnImpulse;

	UPROPERTY(EditAnywhere)
	bool bRandomizeSpawnImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomizeSpawnImpulse"))
	FVector MinSpawnImpulseVariance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomizeSpawnImpulse"))
	FVector MaxSpawnImpulseVariance;

	FSpawnerParameters()
	{
		bCanSpawn = true;
		SpawnTime = 5.0f;
		SpawnRadius = 0.0f;
		SpawnLimit = 1;
		BaseSpawnImpulse = FVector::Zero();
		bRandomizeSpawnImpulse = false;
		MinSpawnImpulseVariance = FVector::Zero();
		MaxSpawnImpulseVariance = FVector::Zero();
	}
};

UCLASS()
class BOMBTHROW_API ABTInteractableSpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABTInteractableSpawnerBase();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "BombThrower|Spawning")
	void SpawnInteractable();
	TArray<AActor*> FindSpawnedInteractables();
	void LaunchInteractable(UStaticMeshComponent* InteractableMesh);

	UFUNCTION(BlueprintCallable, Category = "BombThrower|Spawning")
	void SetSpawnParameters(FSpawnerParameters NewSpawnParameters);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BombThrower|Spawning")
	void OnParametersUpdated();
	void OnParametersUpdated_Implementation();

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AInteractableBase> InteractableToSpawn;
	TArray<AActor*> SpawnedInteractables;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	FSpawnerParameters InitialSpawnParameters;
	UPROPERTY(BlueprintReadOnly, Category = "Spawning")
	FSpawnerParameters CurrentSpawnParameters;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
