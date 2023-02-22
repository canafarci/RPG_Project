// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGCharacter.generated.h"

UCLASS()
class RPG_PROJECT_API ARPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void MoveHorizontal(float Value);
private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* ViewCamera;
};
