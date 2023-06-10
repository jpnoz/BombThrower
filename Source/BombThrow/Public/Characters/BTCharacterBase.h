// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DamageableBase.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BTCharacterBase.generated.h"

UCLASS()
class BOMBTHROW_API ABTCharacterBase : public ADamageableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABTCharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombThrower|Components")
	class UBTMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombThrower|Components")
	class UBTInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombThrower|Components")
	class UBTCharacterInputComponent* CharacterInputComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombThrower|Components")
	class UBTAnimationControlComponent* AnimationControlComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombThrower|Components")
	class UBTDebugComponent* DebugComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
