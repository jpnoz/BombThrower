// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveableBase.generated.h"

UCLASS()
class BOMBTHROW_API AMoveableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveableBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable")
	class UStaticMeshComponent* MoveableMesh;
};
