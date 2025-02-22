// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BTInteractionComponent.h"

// Sets default values for this component's properties
UBTInteractionComponent::UBTInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ThrowForce = 1600.0f;
	PickUpRange = 100.0f;
	bIsHoldingInteractable = false;
}

// Called every frame
void UBTInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateHeldInteractable();
}

void UBTInteractionComponent::UpdateHeldInteractable()
{
	TArray<AActor*> HeldInteractables;
	GetOwner()->GetAttachedActors(HeldInteractables);

	for (AActor* HeldInteractable : HeldInteractables)
	{
		if (!HeldInteractable || HeldInteractable->IsPendingKill())
		{
			HeldInteractables.Remove(HeldInteractable);
		}
	}

	bIsHoldingInteractable = true;
	if (HeldInteractables.Num() == 0)
	{
		bIsHoldingInteractable = false;
		CurrentHeldInteractable = NULL;
	}
}

void UBTInteractionComponent::PickUpInteractable(AActor* ActorDetected)
{
	if (!ActorDetected)
	{
		return;
	}

	FVector InteractableLocation = ActorDetected->GetActorLocation();

	if (CurrentHeldInteractable)
	{
		DetachHeldInteractable();
		CurrentHeldInteractable->SetActorLocation(InteractableLocation);
	}

	CurrentHeldInteractable = ActorDetected;
	InteractableMesh = Cast<UStaticMeshComponent>(CurrentHeldInteractable->GetRootComponent());
	AttachHeldInteractable();
}

void UBTInteractionComponent::PutDownInteractable()
{
	DetachHeldInteractable();
}

void UBTInteractionComponent::ThrowInteractable()
{
	DetachHeldInteractable();

	FVector ThrowImpulse = (GetOwner()->GetActorForwardVector() * ThrowForce + FVector(0, 0, ThrowForce)) * InteractableMesh->GetMass();
	InteractableMesh->AddImpulse(ThrowImpulse);
}

void UBTInteractionComponent::AttachHeldInteractable()
{
	if (!CurrentHeldInteractable || !InteractableMesh)
	{
		return;
	}

	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CurrentHeldInteractable->SetActorRotation(GetOwner()->GetActorRotation());
	FVector InteractableMeshScale = CurrentHeldInteractable->GetActorRelativeScale3D();

	FName SocketName = FName(TEXT("HoldSocket"));
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	//CurrentHeldInteractable->AttachToActor(this, AttachmentRules, Socket);
	InteractableMesh->AttachToComponent(GetOwner()->FindComponentByClass<USkeletalMeshComponent>(), AttachmentRules, SocketName);
	InteractableMesh->SetRelativeScale3D(InteractableMeshScale);
}

void UBTInteractionComponent::DetachHeldInteractable()
{
	if (!CurrentHeldInteractable)
	{
		return;
	}

	// Detach held object from player
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	CurrentHeldInteractable->DetachFromActor(DetachmentRules);

	// Rest held object's rotation
	CurrentHeldInteractable->SetActorRotation(FRotator::ZeroRotator);

	// Enable physics and collision
	InteractableMesh->SetSimulatePhysics(true);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

bool UBTInteractionComponent::bBoxTrace_Implementation(FVector StartLocation, FVector EndLocation, FHitResult& TraceResult)
{
	float BoxSize = PickUpRange / 2.5f;
	FCollisionShape TraceBox = FCollisionShape::MakeBox(FVector::One() * BoxSize);
	bool bSurfaceDetected = GetWorld()->SweepSingleByChannel(TraceResult, StartLocation, EndLocation, GetOwner()->GetActorRotation().Quaternion(), ECC_Visibility, TraceBox);

	return bSurfaceDetected;
}

