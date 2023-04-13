// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPG_Project/Enums.h"
#include "InputActionValue.h"
#include "RPGCharacter.generated.h"

//Forward declarations
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UInputComponent;
class UAnimMontage;
class AWeapon;
//class UEnhancedInputComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class RPG_PROJECT_API ARPGCharacter : public ACharacter
{
	GENERATED_BODY()
//FUNCTIONS
public:
	ARPGCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;
	//--Callbacks for input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	//--Play montage functions
	void PlayAttackMontage();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	//--Helper  Functions
	const bool CanAttack();
	const bool CanDisarm();
	const bool CanRearm();
	const void PlayEquipMontage(FName SectionName);

	UFUNCTION(BlueprintCallable)
	void Disarm();
	UFUNCTION(BlueprintCallable)
	void Rearm();	
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	UFUNCTION(BlueprintCallable)
	void ToggleWeaponCollision(bool bEnable);

//VARIABLES
private:
	UPROPERTY(EditAnywhere)
		float InterpolationSpeed;
	FRotator DesiredRotation;
	UPROPERTY(VisibleAnywhere)
		ECharacterEquipState EquipState = ECharacterEquipState::ECES_Unequipped;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_Unoccupied;
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;
	UPROPERTY(VisibleAnywhere, Category = Hair)
		UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
		UGroomComponent* Eyebrows;
	UPROPERTY(VisibleInstanceOnly)
		 AItem* OverlappingItem;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeapon;
	//--Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* EquipMontage;
protected:
	//----//ENHANCED INPUT
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		UInputMappingContext* InputMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
		UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
		UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
		UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
		UInputAction* EquipAction;
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
		UInputAction* AttackAction;

//GETTERS-SETTERS
public:
	FORCEINLINE void SetOverlappingItem(AItem* item) { OverlappingItem = item; }
	FORCEINLINE ECharacterEquipState GetCharacterEquipState() const { return EquipState; }
};
