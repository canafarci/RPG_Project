// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

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

	//Add Enhanced Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(InputMappingContext, 0);
}

void ARPGCharacter::MoveForward(const FInputActionValue& Value)
{	
	float MovementFloat = Value.Get<float>();
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (MovementFloat != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		//get local forward direction of controller using 3d rotation matrix
		const FVector Direction =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, MovementFloat);
	}
}
void ARPGCharacter::MoveHorizontal(const FInputActionValue& Value)
{
	float MovementFloat = Value.Get<float>();
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (MovementFloat != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		//get local right direction of controller using 3d rotation matrix
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, MovementFloat);
	}
}
void ARPGCharacter::Equip(const FInputActionValue& Value)
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
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanRearm())
		{
			PlayEquipMontage(FName("Equip"));
			EquipState = ECharacterEquipState::ECES_EquippedOneHandSword;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}
void ARPGCharacter::Attack(const FInputActionValue& Value)
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
void ARPGCharacter::Disarm()
{
	if (EquippedWeapon == nullptr) return;
	EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
}
void ARPGCharacter::Rearm()
{
	if (EquippedWeapon == nullptr) return;
	EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
}
void ARPGCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void ARPGCharacter::ToggleWeaponCollision(bool bEnable)
{
	if (EquippedWeapon == nullptr) return;
	EquippedWeapon->ToggleHitCollision(bEnable);
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

void ARPGCharacter::Turn(const FInputActionValue& Value)
{
	float TurnFloat = Value.Get<float>();
	AddControllerYawInput(TurnFloat);
}

void ARPGCharacter::LookVertical(const FInputActionValue& Value)
{
	float LookVerticalFloat = Value.Get<float>();
	AddControllerPitchInput(LookVerticalFloat);
}
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	Input->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ARPGCharacter::MoveForward);
	Input->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &ARPGCharacter::MoveHorizontal);
	Input->BindAction(LookVerticalAction, ETriggerEvent::Triggered, this, &ARPGCharacter::LookVertical);
	Input->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Turn);
	Input->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Equip);
	Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Attack);
}

