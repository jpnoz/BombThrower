// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/BTImpactBombBase.h"

#include "Kismet/KismetMathLibrary.h"

ABTImpactBombBase::ABTImpactBombBase()
{
	ArmTimer = 0.5f;
}

void ABTImpactBombBase::BeginPlay()
{
	Super::BeginPlay();

	bIsArmed = false;
	bCanTick = false;

	CurrentArmTime = ArmTimer;
}

// Called every frame
void ABTImpactBombBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CountDownArmTime(DeltaTime);

	if (CurrentArmTime <= 0)
	{
		bIsArmed = true;
	}
}

void ABTImpactBombBase::CountDownArmTime(float DeltaTime)
{
	CurrentArmTime = UKismetMathLibrary::FClamp(CurrentArmTime - DeltaTime, 0, ArmTimer);;
}

void ABTImpactBombBase::StartFuse()
{
	bCanTick = true;
}