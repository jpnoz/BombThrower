// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/InteractableBase.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	SetRootComponent(InteractableMesh);
}

void AInteractableBase::OnInteract_Implementation(AActor* CallingActor)
{

}

void AInteractableBase::OnStartFocus_Implementation()
{

}

void AInteractableBase::OnEndFocus_Implementation()
{

}

