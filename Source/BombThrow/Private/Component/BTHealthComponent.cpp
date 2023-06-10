// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BTHealthComponent.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UBTHealthComponent::UBTHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100;
}

// Called when the game starts
void UBTHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UBTHealthComponent::TakeDamage(float Damage)
{
	int32 AppliedDamage = (int32)Damage;
	CurrentHealth = UKismetMathLibrary::FClamp(CurrentHealth - Damage, 0, MaxHealth);

	if (CurrentHealth <= 0)
	{
		KillCharacter();
	}
}

void UBTHealthComponent::KillCharacter()
{
	UCapsuleComponent* CharacterRoot = Cast<UCapsuleComponent>(GetOwner()->GetRootComponent());
	CharacterRoot->SetSimulatePhysics(true);
}