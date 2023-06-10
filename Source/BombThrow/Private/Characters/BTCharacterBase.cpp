// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BTCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Component/BTMovementComponent.h"
#include "Component/BTInteractionComponent.h"
#include "Component/BTCharacterInputComponent.h"
#include "Component/BTAnimationControlComponent.h"
#include "Component/BTDebugComponent.h"

// Sets default values
ABTCharacterBase::ABTCharacterBase()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(SpringArm);

	MovementComponent = CreateDefaultSubobject<UBTMovementComponent>(TEXT("PlayerMovement"));
	InteractionComponent = CreateDefaultSubobject<UBTInteractionComponent>(TEXT("PlayerInteraction"));
	CharacterInputComponent = CreateDefaultSubobject<UBTCharacterInputComponent>(TEXT("PlayerInput"));
	AnimationControlComponent = CreateDefaultSubobject<UBTAnimationControlComponent>(TEXT("AnimControl"));
	DebugComponent = CreateDefaultSubobject<UBTDebugComponent>(TEXT("Debug"));
}

void ABTCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void ABTCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ABTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	CharacterInputComponent->SetupInput(PlayerInputComponent);
}

