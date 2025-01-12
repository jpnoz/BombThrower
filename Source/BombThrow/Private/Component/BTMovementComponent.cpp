// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BTMovementComponent.h"

#include "GameFramework/PawnMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UBTMovementComponent::UBTMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	BaseTurnRate = 2.0f;
	SurfaceDetectionRange = 100.0f;
	AlignmentInterpRate = 15.0f;
}

// Called when the game starts
void UBTMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner()->FindComponentByClass<UPawnMovementComponent>()->GetPawnOwner();
}

// Called every frame
void UBTMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()->GetRootComponent()->IsSimulatingPhysics())
	{
		AlignToFloor(DeltaTime);
	}
}

void UBTMovementComponent::Throttle(const float Value)
{
	if (Owner->GetController() && Value != 0.0f)
	{
		const FRotator Rotation = Owner->GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		Owner->AddMovementInput(Direction, Value);
	}
}

void UBTMovementComponent::Rotate(const float Value)
{
	if (Owner->GetController() && Value != 0.0f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%f"), Value * BaseTurnRate);
		
		// Getting delta seconds outside of tick might not be good
		FRotator NewYaw(0, Value * BaseTurnRate * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0);
		Owner->AddActorLocalRotation(NewYaw, false, 0, ETeleportType::None);
	}
}

void UBTMovementComponent::AlignToFloor(float DeltaTime)
{
	FVector CharacterLocation = Owner->GetActorLocation();
	FVector DetectionEnd = CharacterLocation + FVector(0, 0, -SurfaceDetectionRange);
	FHitResult Detection;
	bool bSurfaceDetected = bLineTrace(CharacterLocation, DetectionEnd, Detection);

	if (bSurfaceDetected)
	{
		FRotator CurrentRotation = Owner->GetActorRotation();
		double NewPitch = UKismetMathLibrary::MakeRotFromYZ(Owner->GetActorRightVector(), Detection.ImpactNormal).Pitch;
		double NewRoll = UKismetMathLibrary::MakeRotFromXZ(Owner->GetActorForwardVector(), Detection.ImpactNormal).Roll;

		FRotator TargetRotation = FRotator(NewPitch, CurrentRotation.Yaw, NewRoll);

		if (TargetRotation != CurrentRotation)
		{
			Owner->SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, AlignmentInterpRate));
		}
	}
}

bool UBTMovementComponent::bLineTrace_Implementation(FVector StartLocation, FVector EndLocation, FHitResult& TraceResult)
{
	FCollisionQueryParams TraceParams;
	bool bSurfaceDetected = GetWorld()->LineTraceSingleByChannel(TraceResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	return bSurfaceDetected;
}