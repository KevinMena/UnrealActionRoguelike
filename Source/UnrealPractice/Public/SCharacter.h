// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USAttributesComponent;

UCLASS()
class UNREALPRACTICE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
    TSubclassOf<AActor> AbilityProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
    TSubclassOf<AActor> AbilityClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_FirstAbility;
	FTimerHandle TimerHandle_SecondAbility;

	float LineDistance = 100000;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
    UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    USAttributesComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack();
	void PrimaryInteract();

	void FirstAbility();
	void SecondAbility();

	UFUNCTION()
	void ShootProjectile_TimeElapsed(TSubclassOf<AActor> PClass);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
