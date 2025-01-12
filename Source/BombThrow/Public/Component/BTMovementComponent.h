// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BTMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOMBTHROW_API UBTMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBTMovementComponent();

	void Throttle(const float Value);
	void Rotate(const float Value);
	void AlignToFloor(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BombThrower|Custom Trace")
	bool bLineTrace(FVector StartLocation, FVector EndLocation, FHitResult& TraceResult);
	bool bLineTrace_Implementation(FVector StartLocation, FVector EndLocation, FHitResult& TraceResult);

	// Movement values

	// Rotation speed, in degrees/second
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerMovement")
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerMovement")
	float SurfaceDetectionRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerMovement")
	float AlignmentInterpRate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	APawn* Owner;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
