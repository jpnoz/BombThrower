// Fill out your copyright notice in the Description page of Project Settings.


#include "Moveable/MoveableBase.h"

// Sets default values
AMoveableBase::AMoveableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    MoveableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoveableMesh"));
    SetRootComponent(MoveableMesh);
}

