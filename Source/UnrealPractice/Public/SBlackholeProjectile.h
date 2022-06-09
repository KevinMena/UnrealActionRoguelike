// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SBlackholeProjectile.generated.h"

class URadialForceComponent;
class USphereComponent;

/**
 * 
 */
UCLASS()
class UNREALPRACTICE_API ASBlackholeProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASBlackholeProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    URadialForceComponent* RadialForceComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* SphereDestroyComp;
};
