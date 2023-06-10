// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BTAnimationFlag.generated.h"

UCLASS()
class BOMBTHROW_API UBTAnimationFlag : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, Category = "BombThrower|Animation|Flags")
	FName FlagName;

	UPROPERTY(BlueprintReadWrite, DisplayName = "Is Flag Raised", Category = "BombThrower|Animation|Flags")
	bool bIsFlagRaised;
};
