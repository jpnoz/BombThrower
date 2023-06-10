// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/BTAnimationFlag.h"
#include "BTAnimationControlComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOMBTHROW_API UBTAnimationControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBTAnimationControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeAnimationFlags();
	UBTAnimationFlag* FindAnimationFlag(FName FlagName);

	UFUNCTION(BlueprintNativeEvent, BluePrintCallable, DisplayName = "Get Animation Flag", Category = "BombThrower|Animation")
	bool bGetAnimationFlag(FName FlagName);
	bool bGetAnimationFlag_Implementation(FName FlagName);

	UFUNCTION(BlueprintNativeEvent, BluePrintCallable, Category = "BombThrower|Animation")
	void SetAnimationFlag(FName FlagName, bool FlagValue);
	void SetAnimationFlag_Implementation(FName FlagName, bool FlagValue);

	UPROPERTY(EditAnywhere, Category = "BombThrower|Animation")
	TArray<FName> AnimationFlagNames;
	TArray<UBTAnimationFlag*> AnimationFlags;
};