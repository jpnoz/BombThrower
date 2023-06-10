// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BTHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOMBTHROW_API UBTHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBTHealthComponent();

	// Gamplay values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	int32 MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	int32 CurrentHealth;

	UFUNCTION(BlueprintCallable, Category = "BombThrower|Damage")
	void TakeDamage(float Damage);
	void KillCharacter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
