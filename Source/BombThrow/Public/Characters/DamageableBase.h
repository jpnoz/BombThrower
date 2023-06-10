// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageableBase.generated.h"

UCLASS()
class BOMBTHROW_API ADamageableBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADamageableBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damageable")
	class UBTHealthComponent* HealthComponent;
};
