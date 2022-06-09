// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SInteractionComponent.h"
#include "SAttributesComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleEventManager.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributesComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	LineStart += GetActorRightVector() * 100.0f;
	FVector ActionDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActionDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement 
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// Rotation
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Actions
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	// Abilities
	PlayerInputComponent->BindAction("FirstAbility", IE_Pressed, this, &ASCharacter::FirstAbility);
	PlayerInputComponent->BindAction("SecondAbility", IE_Pressed, this, &ASCharacter::SecondAbility);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ShootProjectile_TimeElapsed"), ProjectileClass);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, TimerDelegate, 0.2f, false);
}

void ASCharacter::PrimaryInteract()
{
	if(InteractionComp)
	    InteractionComp->PrimaryInteract();
}

void ASCharacter::FirstAbility()
{
	PlayAnimMontage(AttackAnim);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ShootProjectile_TimeElapsed"), AbilityProjectileClass);
	GetWorldTimerManager().SetTimer(TimerHandle_FirstAbility, TimerDelegate, 0.2f, false);
}

void ASCharacter::SecondAbility()
{
	PlayAnimMontage(AttackAnim);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ShootProjectile_TimeElapsed"), AbilityClass);
	GetWorldTimerManager().SetTimer(TimerHandle_SecondAbility, TimerDelegate, 0.2f, false);
}

void ASCharacter::ShootProjectile_TimeElapsed(TSubclassOf<AActor> PClass)
{
	if (ensure(PClass))
	{
		// Add the collision queries we want
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		// Get the camera location and rotation
		APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector CameraLocation = CameraManager->GetCameraLocation();
		FRotator CameraRotation = CameraManager->GetCameraRotation();

		FVector End = CameraLocation + (CameraRotation.Vector() * LineDistance);

		// Throw a LineTrace from the camera to the world
		FHitResult Hit;
		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, End, ObjectQueryParams);

		FVector LookAtLocation = End;

		if (bBlockingHit)
			LookAtLocation = Hit.ImpactPoint;

		// Debug things
		FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), CameraLocation, End, LineColor, false, 2.0f, 0, 2.0f);

		FVector HandPosition = GetMesh()->GetSocketLocation("Muzzle_01");

		// Calculate new rotation of the spawn object base on the line trace
		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandPosition, LookAtLocation);

		FTransform SpawnTM = FTransform(SpawnRotation, HandPosition);

		// Set Spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(PClass, SpawnTM, SpawnParams);
	}
}

