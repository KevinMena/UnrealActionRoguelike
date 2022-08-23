// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

//* Add audio playback(SoundCues from lecture)
//* Looped ‘flight’ sound(UAudioComponent)
//* Impact sound(UGameplayStatics::PlaySoundAtLocation)
class UAudioComponent;

UCLASS()
class UNREALPRACTICE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Components")
	UAudioComponent* SoundComp;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
    USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
public:	
	ASMagicProjectile();
};
