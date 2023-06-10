// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BTCharacterInputComponent.h"

#include "Component/BTMovementComponent.h"
#include "Component/BTDebugComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UBTCharacterInputComponent::UBTCharacterInputComponent()
{
	BasePitchRate = 25.0f;
	MinPitch = -10.0f;
	MaxPitch = 70.0f;

	BaseZoomIncrement = 100.0f;
	MinZoom = 200.0f;
	MaxZoom = 3000.0f;
}

// Called when the game starts
void UBTCharacterInputComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner()->FindComponentByClass<UPawnMovementComponent>()->GetPawnOwner();

	APlayerController* PlayerController = Cast<APlayerController>(Owner->GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void UBTCharacterInputComponent::Throttle(const FInputActionValue& ActionValue)
{
	const float Value = ActionValue.Get<FVector>().X;
	GetOwner()->FindComponentByClass<UBTMovementComponent>()->Throttle(Value);
}

void UBTCharacterInputComponent::Rotate(const FInputActionValue& ActionValue)
{
	const float Value = ActionValue.Get<FVector>().X;
	GetOwner()->FindComponentByClass<UBTMovementComponent>()->Rotate(Value);
}

void UBTCharacterInputComponent::CameraPitch(const FInputActionValue& ActionValue)
{
	const float Value = ActionValue.Get<FVector>().X;
	Owner->AddControllerPitchInput(Value);
}

//void ABTCharacterBase::CameraPitchAtRate(float Value)
//{
//	AddControllerPitchInput(Value * BasePitchRate * GetWorld()->GetDeltaSeconds());
//}

void UBTCharacterInputComponent::CameraZoom(const FInputActionValue& ActionValue)
{
	const float Value = ActionValue.Get<FVector>().X;
	USpringArmComponent* SpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>();
	float NewArmLengh = SpringArm->TargetArmLength - Value * BaseZoomIncrement;
	SpringArm->TargetArmLength = UKismetMathLibrary::Clamp(NewArmLengh, MinZoom, MaxZoom);
}

void UBTCharacterInputComponent::Debug_SpawnInteractable()
{
	GetOwner()->FindComponentByClass<UBTDebugComponent>()->SpawnObject();
}

void UBTCharacterInputComponent::SetupInput(UInputComponent* PlayerInputComponent)
{
	// Cast input component to enhanced input
	// Crash out if unsuccessful
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind actions to functions
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &UBTCharacterInputComponent::Throttle);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &UBTCharacterInputComponent::Rotate);
		EnhancedInputComponent->BindAction(CameraPitchAction, ETriggerEvent::Triggered, this, &UBTCharacterInputComponent::CameraPitch);
		EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &UBTCharacterInputComponent::CameraZoom);
		EnhancedInputComponent->BindAction(SpawnInteractableAction, ETriggerEvent::Started, this, &UBTCharacterInputComponent::Debug_SpawnInteractable);
	}

	//PlayerInputComponent->BindAxis("LookPitchRate", this, &ABTCharacterBase::CameraPitchAtRate);
}