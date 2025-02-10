// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DamageableBase.h"
#include "GameFramework/Character.h"
#include "BTDefendObjectiveBase.generated.h"

UCLASS()
class BOMBTHROW_API ABTDefendObjectiveBase : public ADamageableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABTDefendObjectiveBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombThrower|Components")
	class UBTDefendObjectiveComponent* DefendObjectiveComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
