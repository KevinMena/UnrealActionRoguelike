// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASBlackholeProjectile::ASBlackholeProjectile() : ASProjectileBase()
{
    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
    RadialForceComp->SetupAttachment(SphereComp);

    SphereDestroyComp = CreateDefaultSubobject<USphereComponent>("SphereDestroyComp");
    SphereDestroyComp->SetupAttachment(SphereComp);
}
