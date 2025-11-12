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

	UPROPERTY(BlueprintReadWrite, Category = "GameState|Objective")
	TArray<AActor*> AllPlayers;

	UPROPERTY(BlueprintReadWrite, Category = "GameState|Objective")
	TArray<AActor*> AllEnemies;

	UPROPERTY(BlueprintReadWrite, Category = "GameState|Objective")
	TArray<AActor*> AllDefendObjectives;

	TArray<AActor*> AllInteractables;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameState|Objective")
	void OnPlayerDefeated(AActor* DefeatedPlayer);
	void OnPlayerDefeated_Implementation(AActor* DefeatedPlayer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameState|Player")
	void OnEnemyDefeated(AActor* DefeatedEnemy);
	void OnEnemyDefeated_Implementation(AActor* DefeatedEnemy);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameState|Enemy")
	void OnDefendObjectiveDestroyed(AActor* DestroyedObjective);
	void OnDefendObjectiveDestroyed_Implementation(AActor* DestroyedObjective);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
