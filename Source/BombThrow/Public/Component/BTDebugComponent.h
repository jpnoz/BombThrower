// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BTDebugComponent.generated.h"

class AInteractableBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOMBTHROW_API UBTDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBTDebugComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|Spawning")
	TSubclassOf<AInteractableBase> InteractableToSpawn;

	UFUNCTION()
	void SpawnObject();

protected:

	// Debug values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|Spawning")
	bool bHoldSpawnedInteractable;

	// Non-editable variables
	UPROPERTY(BlueprintReadOnly, Category = "BombThrower|Interaction")
	AActor* CurrentHeldInteractable;
	UStaticMeshComponent* InteractableMesh;
};
