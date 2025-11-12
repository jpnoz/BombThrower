// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/BTBombBase.h"

#include "Game/BTGameStateBase.h"
#include "Components/CapsuleComponent.h"
#include "Characters/DamageableBase.h"
#include "Characters/BTCharacterBase.h"
#include "Interactable/BTInteractableSpawnerBase.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "DrawDebugHelpers.h"

ABTBombBase::ABTBombBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BombMaterial = InteractableMesh->GetMaterial(1);

	BaseBombColor = FLinearColor::FLinearColor(0.017f, 0.017f, 0.017f, 1.0f);
	FinalBombColor = FLinearColor::FLinearColor(0.5f, 0.017f, 0.017f, 1.0f);
	
	ParticleRenderDistance = 10000.0f;
	bShouldSpawnParticle = false;

	FuseTimer = 8.0f;
	BlastRadius = 1000.0f;
	ExplosionForce = 2000.0f;
	MinExplosionDamage = 10.0f;
	MaxExplosionDamage = 70.0f;

	bCanTick = true;
	bFreezeOnPlayerHold = true;
}

void ABTBombBase::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = InteractableMesh->CreateDynamicMaterialInstance(1, BombMaterial);
	InteractableMesh->SetMaterial(1, DynamicMaterial);

	CurrentBombColor = BaseBombColor;

	CurrentFuse = FuseTimer;
	bIsTicking = bCanTick;
}

// Called every frame
void ABTBombBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFreezeOnPlayerHold && bCanTick)
	{
		bIsTicking = GetAttachParentActor() == NULL || !GetAttachParentActor()->IsA(ABTCharacterBase::StaticClass());
	}

	// Ensure correct rotation
	SetActorRotation(FRotator(0, 0, 0));

	// Update bomb color based on remaining fuse
	DynamicMaterial->SetVectorParameterValue("Color", FVector4(CurrentBombColor));

	// Tick down fuse every frame
	if (bIsTicking)
	{
		CountDown(DeltaTime);
	}
}

void ABTBombBase::CountDown(float DeltaTime)
{
	CurrentFuse = UKismetMathLibrary::FClamp(CurrentFuse - DeltaTime, 0, FuseTimer);

	// Interpolate color values (coords in vector) as timer approaches 0
	float FusePercent = 2.0f / (1 + FMath::Exp(CurrentFuse));
	CurrentBombColor = UKismetMathLibrary::LinearColorLerp(BaseBombColor, FinalBombColor, FusePercent);

	if (CurrentFuse <= 0)
	{
		bCanTick = false;
		Explode();
	}
}

void ABTBombBase::Explode()
{
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachmentRules);

	PushActorsInRadius();

	if (ExplosionParticle)
	{
		if (bShouldSpawnParticle)
		{
			UNiagaraComponent* ExplosionParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionParticle, GetActorLocation());
			ExplosionParticleComponent->SetNiagaraVariableFloat(FString("User.ParticleScale"), BlastRadius / 50.0f);
		}
	}
	else
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), BlastRadius, 32, FColor::Red, false, 1.0f);
	}
	
	Destroy();
}

void ABTBombBase::PushActorsInRadius()
{
	// Separate Moveable and Damageable Actors from other Actors
	// For rendering particles and to handle damage correctly
	TArray<AActor*> AllMoveables;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMoveableBase::StaticClass(), AllMoveables);

	TArray<AActor*> AllDamageables;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADamageableBase::StaticClass(), AllDamageables);

	AllMoveables += AllDamageables;

	// Sphere Trace for all actors in blast radius
	// Instead of GetAllActorsWithTag() to prevent crash when too many bombs exist at once
	ABTGameStateBase* GameState = Cast<ABTGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Found: %d"), MoveableActors.Num()));
	}*/

	// Checking general Actors first
	for (int i = 0; i < GameState->AllInteractables.Num(); i++)
	{
		AActor* ActorToCheck = GameState->AllInteractables[i];

		if (ActorToCheck == this || ActorToCheck->Tags.Num() == 0 || (ActorToCheck->GetAttachParentActor() && !ActorToCheck->GetAttachParentActor()->IsA(ABTInteractableSpawnerBase::StaticClass())))
		{
			continue;
		}

		FVector DistanceToActor = ActorToCheck->GetActorLocation() - GetActorLocation();

		if (DistanceToActor.SquaredLength() > (BlastRadius * BlastRadius))
		{
			continue;
		}

		UMeshComponent* ActorMesh = Cast<UMeshComponent>(ActorToCheck->GetRootComponent());
		ActorMesh->SetSimulatePhysics(true);
		ActorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		if (ActorMesh->IsSimulatingPhysics())
		{
			DistanceToActor.Normalize(1.0f);
			FVector ExplosionImpulse = DistanceToActor * ExplosionForce * ActorMesh->GetMass();
			ActorMesh->AddImpulse(ExplosionImpulse);
		}
	}

	// Checking Moveables and Damageables
	for (int i = 0; i < AllMoveables.Num(); i++)
	{
		AActor* ActorToCheck = AllMoveables[i];
		FVector DistanceToActor = ActorToCheck->GetActorLocation() - GetActorLocation();
		
		if (DistanceToActor.SquaredLength() <= BlastRadius * BlastRadius)
		{
			bShouldSpawnParticle = true;
			PushMoveable(ActorToCheck, DistanceToActor);
			continue;
		}

		if (!bShouldSpawnParticle && DistanceToActor.SquaredLength() <= ParticleRenderDistance * ParticleRenderDistance)
		{
			bShouldSpawnParticle = true;
		}
	}
}

void ABTBombBase::PushMoveable(AActor* MoveableActor, FVector VectorToActor)
{
	if (MoveableActor->IsA(ADamageableBase::StaticClass()))
	{
		float CalculatedDamage = CalculateDamage(VectorToActor);
		DamageableHit(MoveableActor, CalculatedDamage);
	}

	UPrimitiveComponent* MeshComp = Cast<UPrimitiveComponent>(MoveableActor->GetRootComponent());
	if (MeshComp->IsSimulatingPhysics())
	{
		VectorToActor.Normalize(1.0f);
		FVector ExplosionImpulse = (VectorToActor + FVector::UnitZ() * 0.5f) * ExplosionForce * MeshComp->GetMass();
		MeshComp->AddImpulse(ExplosionImpulse);
	}
}

void ABTBombBase::DamageableHit_Implementation(AActor* DamageableActor, float Damage)
{
}

float ABTBombBase::CalculateDamage(FVector VectorToActor)
{
	float ExplosionDamage = MaxExplosionDamage;

	if (MinExplosionDamage != MaxExplosionDamage)
	{
		float DistanceToActor = VectorToActor.Length();
		float ExplosionDamageDropoff = (BlastRadius - 50) / (MaxExplosionDamage - MinExplosionDamage);
		ExplosionDamage = MaxExplosionDamage - ((DistanceToActor - 50) / ExplosionDamageDropoff);
	}

	return ExplosionDamage;
}

// ActorsToIgnore has to be an address
// Maybe passing an array as an out param forces that?
bool ABTBombBase::bSphereTrace_Implementation(FVector Location, TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& HitActors)
{
	//FCollisionShape TraceSphere = FCollisionShape::MakeSphere(BlastRadius);
	//bool bActorHit = GetWorld()->SweepMultiByChannel(TraceResults, Location, Location, FQuat::Identity, ECC_Visibility, TraceSphere);

	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;
	TargetObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Append(ActorsToIgnore);

	bool bActorHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Location, BlastRadius, TargetObjectTypes, NULL, IgnoredActors, HitActors);
	return bActorHit;
}