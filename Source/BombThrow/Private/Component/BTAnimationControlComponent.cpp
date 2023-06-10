// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BTAnimationControlComponent.h"

#include "Data/BTAnimationFlag.h"

// Sets default values for this component's properties
UBTAnimationControlComponent::UBTAnimationControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UBTAnimationControlComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeAnimationFlags();
}

void UBTAnimationControlComponent::InitializeAnimationFlags()
{
	for (FName AnimFlagName : AnimationFlagNames)
	{
		UBTAnimationFlag* NewFlag = NewObject<UBTAnimationFlag>(this, UBTAnimationFlag::StaticClass());

		NewFlag->FlagName = AnimFlagName;
		NewFlag->bIsFlagRaised = false;

		AnimationFlags.Add(NewFlag);
	}
}

UBTAnimationFlag* UBTAnimationControlComponent::FindAnimationFlag(FName FlagName)
{
	UBTAnimationFlag* FoundFlag = nullptr;

	for (UBTAnimationFlag* AnimationFlag : AnimationFlags)
	{
		if (AnimationFlags.Num() < 1 || !AnimationFlag || !AnimationFlag->FlagName.IsValid() || !FlagName.IsValid() )
		{
			break;
		}

		if (AnimationFlag->FlagName.Compare(FlagName) == 0)
		{
			FoundFlag = AnimationFlag;
			break;
		}
	}
	
	return FoundFlag;
}

bool UBTAnimationControlComponent::bGetAnimationFlag_Implementation(FName FlagName)
{
	UBTAnimationFlag* FoundFlag = FindAnimationFlag(FlagName);

	if (!FoundFlag)
	{
		return false;
	}
	
	return FoundFlag->bIsFlagRaised;
}

void UBTAnimationControlComponent::SetAnimationFlag_Implementation(FName FlagName, bool FlagValue)
{
	UBTAnimationFlag* FoundFlag = FindAnimationFlag(FlagName);

	if (FoundFlag == nullptr)
	{
		return;
	}

	FoundFlag->bIsFlagRaised = FlagValue;
}
