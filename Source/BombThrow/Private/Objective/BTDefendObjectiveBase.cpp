// Fill out your copyright notice in the Description page of Project Settings.


#include "Objective/BTDefendObjectiveBase.h"

#include "Component/Objective/BTDefendObjectiveComponent.h"

// Sets default values
ABTDefendObjectiveBase::ABTDefendObjectiveBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefendObjectiveComponent = CreateDefaultSubobject<UBTDefendObjectiveComponent>(TEXT("DefendObjectiveControl"));
}

// Called when the game starts or when spawned
void ABTDefendObjectiveBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABTDefendObjectiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABTDefendObjectiveBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

