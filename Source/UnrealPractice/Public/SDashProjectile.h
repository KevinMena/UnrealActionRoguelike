// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"


UCLASS()
class UNREALPRACTICE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
    float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
    float DetonateDelay;

	FTimerHandle TimerHandle_DelayedExplosion;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ASDashProjectile();
};
