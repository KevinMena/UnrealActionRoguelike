// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

class UParticleSystemComponent;

UCLASS()
class UNREALPRACTICE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
    UParticleSystemComponent* ExplosionParticleComp;

	FTimerHandle TimerHandle_Explode;
	FTimerHandle TimerHandler_Teleport;

public:
	// Sets default values for this actor's properties
	ASDashProjectile();

protected:
	virtual void BeginPlay() override;

	void Explode();

    UFUNCTION()
	void Explode_TimeElapsed();

	UFUNCTION()
    void Teleport_TimeElapsed();

	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
