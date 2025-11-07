// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Objective/BTDefendObjectiveBase.h"
#include "BTGameStateBase.generated.h"

UCLASS()
class BOMBTHROW_API ABTGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABTGameStateBase();

	const int32 MaxInteractables = 2048;

	TArray<AActor*> AllPlayers;
	TArray<AActor*> AllInteractables;
	UPROPERTY(BlueprintReadWrite, Category = "GameState|Objective")
	TArray<AActor*> AllDefendObjectives;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GetAllPlayerActors();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameState|Objective")
	void OnDefendObjectiveDestroyed(AActor* DestroyedObjective);
	void OnDefendObjectiveDestroyed_Implementation(AActor* DestroyedObjective);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
