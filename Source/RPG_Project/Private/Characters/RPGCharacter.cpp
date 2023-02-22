// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ARPGCharacter::ARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(CameraBoom);
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGCharacter::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		//get local forward direction of controller using 3d rotation matrix
		const FVector Direction =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void ARPGCharacter::MoveHorizontal(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		//get local right direction of controller using 3d rotation matrix
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
void ARPGCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ARPGCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}
// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ARPGCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveHorizontal"), this, &ARPGCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ARPGCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ARPGCharacter::LookUp);
}

