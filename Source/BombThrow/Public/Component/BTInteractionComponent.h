// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BTInteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOMBTHROW_API UBTInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBTInteractionComponent();

	void AttachHeldInteractable();
	void DetachHeldInteractable();

	// For Animation Blueprints
	UPROPERTY(BlueprintReadOnly, Category = "BombThrower|Interaction")
	bool bIsHoldingInteractable;

protected:
	void UpdateHeldInteractable();

	// Called by animation notifies, easier to do in BPs
	UFUNCTION(BluePrintCallable, Category = "BombThrower|Interaction")
	void PickUpInteractable(AActor* ActorDetected);

	UFUNCTION(BluePrintCallable, Category = "BombThrower|Interaction")
	void PutDownInteractable();

	UFUNCTION(BluePrintCallable, Category = "BombThrower|Interaction")
	void ThrowInteractable();

	UFUNCTION(BlueprintNativeEvent, BluePrintCallable, DisplayName = "Box Trace", Category = "BombThrower|Custom Trace")
	bool bBoxTrace(FVector StartLocation, FVector EndLocation, FHitResult& TraceResult);
	bool bBoxTrace_Implementation(FVector StartLocation, FVector EndLocation, FHitResult& TraceResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float ThrowForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float PickUpRange;

	// Non-editable variables
	UPROPERTY(BlueprintReadOnly, Category = "BombThrower|Interaction")
	AActor* CurrentHeldInteractable;
	UStaticMeshComponent* InteractableMesh;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
