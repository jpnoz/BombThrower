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
	UE_LOG(LogTemp, Warning, TEXT("Character has died"));
	// Get all Children that make up Character Model
	UCapsuleComponent* CharacterRoot = Cast<UCapsuleComponent>(GetOwner()->GetRootComponent());
	TArray<USceneComponent*> CharacterMeshes;
	CharacterRoot->GetChildrenComponents(false, CharacterMeshes);

	// Loop through each Child, focusing on Meshes
	for (int i = 0; i < CharacterMeshes.Num(); i++)
	{
		// Enable Physics Simulation for all Static/Skeletal Meshes found
		USceneComponent* ChildToCheck = CharacterMeshes[i];
		if (ChildToCheck->IsA(UStaticMeshComponent::StaticClass()))
		{
			UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(ChildToCheck);
			StaticMesh->SetSimulatePhysics(true);
		}
		else if (ChildToCheck->IsA(USkeletalMeshComponent::StaticClass()))
		{
			CharacterRoot->SetSimulatePhysics(true);
		}
	}
}