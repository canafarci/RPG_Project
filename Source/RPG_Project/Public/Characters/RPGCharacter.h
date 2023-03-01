// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPG_Project/Enums.h"
#include "RPGCharacter.generated.h"

//Forward declarations
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UInputComponent;
class UAnimMontage;

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
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void MoveHorizontal(float Value);
	void Equip();
	void Attack();
	//--Play montage functions
	void PlayAttackMontage();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	//--Helper  Functions
	const bool CanAttack();

//VARIABLES
private:
	UPROPERTY(VisibleAnywhere)
		ECharacterEquipState EquipState = ECharacterEquipState::ECES_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
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
	//--Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* AttackMontage;

//GETTERS-SETTERS
public:
	FORCEINLINE void SetOverlappingItem(AItem* item) { OverlappingItem = item; }
	FORCEINLINE ECharacterEquipState GetCharacterEquipState() const { return EquipState; }
};
