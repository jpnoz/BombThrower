// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BTGameStateBase.h"

#include "Interactable/InteractableBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABTGameStateBase::ABTGameStateBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

void ABTGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractableBase::StaticClass(), AllInteractables);
}