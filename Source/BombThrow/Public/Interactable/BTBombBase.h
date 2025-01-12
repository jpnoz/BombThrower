// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableBase.h"
#include "GameplayTagContainer.h"
#include "BTBombBase.generated.h"

class UNiagaraSystem;

UCLASS()
class BOMBTHROW_API ABTBombBase : public AInteractableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABTBombBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CountDown(float DeltaTime);
	void Explode();
	void PushActorsInRadius();
	void PushMoveable(AActor* MoveableActor, FVector VectorToActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BombThrower|Damage")
	void DamageableHit(AActor* DamageableActor, float Damage);
	void DamageableHit_Implementation(AActor* DamageableActor, float Damage);
	float CalculateDamage(FVector VectorToActor);

	UFUNCTION(BlueprintNativeEvent, BluePrintCallable, DisplayName = "Sphere Trace", Category = "BombThrower|Custom Trace")
	bool bSphereTrace(FVector Location, TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& HitActors);
	bool bSphereTrace_Implementation(FVector Location, TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& HitActors);

	UMaterialInterface* BombMaterial;
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Appearance")
	FLinearColor BaseBombColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Appearance")
	FLinearColor FinalBombColor;
	FLinearColor CurrentBombColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Appearance")
	UNiagaraSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Appearance")
	float ParticleRenderDistance;
	bool bShouldSpawnParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Explosion")
	float FuseTimer;
	float CurrentFuse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Explosion")
	bool bCanTick;
	bool bIsTicking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Explosion")
	bool bFreezeOnPlayerHold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Explosion")
	float BlastRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Explosion")
	float ExplosionForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Explosion")
	float MinExplosionDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb|Explosion")
	float MaxExplosionDamage;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
