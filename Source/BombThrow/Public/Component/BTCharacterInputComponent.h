// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "BTCharacterInputComponent.generated.h"

class UInputAction;

UCLASS()
class BOMBTHROW_API UBTCharacterInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UBTCharacterInputComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	void Throttle(const FInputActionValue& ActionValue);
	void Rotate(const FInputActionValue& ActionValue);
	void CameraPitch(const FInputActionValue& ActionValue);
	//void CameraPitchAtRate(float Value);
	void CameraZoom(const FInputActionValue& ActionValue);
	void Debug_SpawnInteractable();

	void SetupInput(UInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* MappingContext;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CameraPitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CameraZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SpawnInteractableAction;

	// Camera values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BasePitchRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "-30.0", ClampMax = "0.0", UIMin = "-30.0", UIMax = "0.0"), Category = "Camera")
	float MinPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "90.0", UIMin = "0.0", UIMax = "90.0"), Category = "Camera")
	float MaxPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseZoomIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinZoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxZoom;

protected:

	APawn* Owner;
};
