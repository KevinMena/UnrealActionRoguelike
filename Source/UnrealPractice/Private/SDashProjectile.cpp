// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


ASDashProjectile::ASDashProjectile() : ASProjectileBase()
{
    TeleportDelay = 0.2f;
    DetonateDelay = 0.2f;

    MovementComp->InitialSpeed = 6000;
}

void ASDashProjectile::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(TimerHandle_DelayedExplosion, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_DelayedExplosion);

    UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

    EffectComp->DeactivateSystem();

    MovementComp->StopMovementImmediately();
    SetActorEnableCollision(false);

    FTimerHandle TimerHandle_DelayedTeleport;
    GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);

}

void ASDashProjectile::TeleportInstigator()
{
    AActor* InstigatorToTeleport = GetInstigator();

    if(ensure(InstigatorToTeleport))
    {
        InstigatorToTeleport->TeleportTo(GetActorLocation(), InstigatorToTeleport->GetActorRotation(), false, false);
    }
}
