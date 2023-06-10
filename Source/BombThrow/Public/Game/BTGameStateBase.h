// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BTGameStateBase.generated.h"

UCLASS()
class BOMBTHROW_API ABTGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABTGameStateBase();

	const int32 MaxInteractables = 2048;

	TArray<AActor*> AllInteractables;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
