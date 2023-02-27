// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPG_Project/Enums.h"
#include "RPGCharacter.generated.h"


UCLASS()
class RPG_PROJECT_API ARPGCharacter : public ACharacter
{
	GENERATED_BODY()
//FUNCTIONS
public:
	ARPGCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void MoveHorizontal(float Value);
	void Equip();
//VARIABLES
private:
	UPROPERTY(VisibleAnywhere)
	ECharacterEquipState EquipState = ECharacterEquipState::ECES_Unequipped;
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* ViewCamera;
	UPROPERTY(VisibleAnywhere, Category = Hair)
		class UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
		UGroomComponent* Eyebrows;
	UPROPERTY(VisibleInstanceOnly)
		 class AItem* OverlappingItem;
//GETTERS-SETTERS
public:
	FORCEINLINE void SetOverlappingItem(AItem* item) { OverlappingItem = item; }
	FORCEINLINE ECharacterEquipState GetCharacterEquipState() const { return EquipState; }
};
