// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DamageableBase.h"

#include "Component/BTHealthComponent.h"

// Sets default values
ADamageableBase::ADamageableBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	HealthComponent = CreateDefaultSubobject<UBTHealthComponent>(TEXT("ActorHealth"));

	if (HealthComponent->MaxHealth > 0)
	{
		bIsAlive = true;
	}
	else
	{
		bIsAlive = false;
	}
}

