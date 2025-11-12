// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Moveable/MoveableBase.h"
#include "GameFramework/Character.h"
#include "DamageableBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageableDeath, AActor*, KilledDamageable);

// TO DO: Find a way to have ADamageableBase extend from AMoveableBase
UCLASS()
class BOMBTHROW_API ADamageableBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADamageableBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damageable")
	class UBTHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damageable")
	bool bIsAlive;

	UPROPERTY(BlueprintAssignable)
	FOnDamageableDeath OnDamageableDeath;

	virtual void OnDeath();

	// Must call Parent Function for Death Handling
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damageable")
	void DamageableDeath();
	void DamageableDeath_Implementation();
};
