// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

/*
 * * Projectile class spawned via input key (same as MagicProjectile) X
* ParticleComponent to show projectile X
* ‘Explodes’ after 0.2 seconds. (Timer) X
* Play particle effect at point of detonation. X
* Waits 0.2 seconds again (Timer) before Teleporting PlayerCharacter (aka the ‘Instigator’ of the projectile) X
* _// Let the detonation effect play before we teleport_  X
* _// Make sure you ‘stop’ the projectile movement while you wait(!)_ X
* _// Look back at ‘Instigator’ we used for ignoring collisions, and re-apply here for teleportation._
* On hit with world: immediately stop movement and execute the same behavior (explode + teleport) X
* Made in C++! (Only applies to the logic, assigning of assets is always done in Blueprint child class)
* * For the Dash Projectile, have a look at Actor->TeleportTo(…) rather than directly setting the Actor’s Location._
* (It tests collision for us to not get stuck in walls and floors)_ X
 */

ASDashProjectile::ASDashProjectile() : ASProjectileBase()
{
    ExplosionParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("ExplosionComp");
    ExplosionParticleComp->SetupAttachment(SphereComp);
    ExplosionParticleComp->bAutoActivate = false;

    SphereComp->SetEnableGravity(false);
    MovementComp->ProjectileGravityScale = 0;
}

void ASDashProjectile::BeginPlay()
{
    Super::BeginPlay();

    SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
    
    Explode();
}

void ASDashProjectile::Explode()
{
    GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASDashProjectile::Explode_TimeElapsed, 0.5);
}

void ASDashProjectile::Explode_TimeElapsed()
{
    EffectComp->DeactivateImmediate();
    ExplosionParticleComp->Activate(true);
    MovementComp->StopMovementImmediately();
    GetWorldTimerManager().SetTimer(TimerHandler_Teleport, this, &ASDashProjectile::Teleport_TimeElapsed, 0.2);
}

void ASDashProjectile::Teleport_TimeElapsed()
{
    APawn* MyInstigator = GetInstigator();

    FVector ProjectileLocation = GetActorLocation();
    FRotator ProjectileRotation = GetActorRotation();

    MyInstigator->TeleportTo(ProjectileLocation, ProjectileRotation);

    Destroy();
}

void ASDashProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    Explode();
}
