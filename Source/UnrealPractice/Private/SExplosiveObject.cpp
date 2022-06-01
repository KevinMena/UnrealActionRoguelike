// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveObject.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveObject::ASExplosiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionProfileName("PhysicsActor");
	MeshComp->SetSimulatePhysics(true);
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveObject::OnHit);
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->Radius = 600.0f;
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void ASExplosiveObject::BeginPlay()
{
	Super::BeginPlay();
}

void ASExplosiveObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
}

// Called every frame
void ASExplosiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

