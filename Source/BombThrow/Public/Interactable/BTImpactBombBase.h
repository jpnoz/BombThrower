// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/BTBombBase.h"
#include "BTImpactBombBase.generated.h"

/**
 * 
 */
UCLASS()
class BOMBTHROW_API ABTImpactBombBase : public ABTBombBase
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	ABTImpactBombBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Explosion")
	float ArmTimer;
	float CurrentArmTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Explosion")
	bool bIsArmed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CountDownArmTime(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "BombThrower|Explosion")
	void StartFuse();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
