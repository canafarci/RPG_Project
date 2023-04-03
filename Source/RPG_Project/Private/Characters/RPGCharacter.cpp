// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

// Sets default values
ARPGCharacter::ARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGCharacter::MoveForward(float Value)
{	
	if (ActionState == EActionState::EAS_Attacking) return;
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
void ARPGCharacter::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon) 
	{
		OverlappingWeapon->EquipWeapon(GetMesh(), FName("RightHandSocket"));
		EquipState = OverlappingWeapon->WeaponEquipState;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			EquipState = ECharacterEquipState::ECES_Unequipped;
		}
		else if (CanRearm())
		{
			PlayEquipMontage(FName("Equip"));
			EquipState = ECharacterEquipState::ECES_EquippedOneHandSword;
		}
	}

}
void ARPGCharacter::Attack()
{
	if (!CanAttack()) return;
	PlayAttackMontage();
	ActionState = EActionState::EAS_Attacking;
}
void ARPGCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr || AttackMontage == nullptr) return;
	//play montage
	AnimInstance->Montage_Play(AttackMontage);
	//randomize which part of the montage is to be played
	FString SectionName = FString("Attack");
	int32 Selection = FMath::RandRange(1, 5);
	SectionName.AppendInt(Selection);
	//play section of the montage
	AnimInstance->Montage_JumpToSection(FName(SectionName));
}
const void ARPGCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr || EquipMontage == nullptr) return;
	AnimInstance->Montage_Play(EquipMontage);
	AnimInstance->Montage_JumpToSection(SectionName);
}
void ARPGCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
const bool ARPGCharacter::CanAttack()
{
	return (ActionState != EActionState::EAS_Attacking && 
			EquipState != ECharacterEquipState::ECES_Unequipped);
}
const bool ARPGCharacter::CanDisarm()
{
	return (EquipState != ECharacterEquipState::ECES_Unequipped &&
			ActionState == EActionState::EAS_Unoccupied);
}
const bool ARPGCharacter::CanRearm()
{
	return (ActionState == EActionState::EAS_Unoccupied && 
			EquipState == ECharacterEquipState::ECES_Unequipped && 
			EquippedWeapon);
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
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ARPGCharacter::Equip);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ARPGCharacter::Attack);
}

