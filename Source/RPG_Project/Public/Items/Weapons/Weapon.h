// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "RPG_Project/Enums.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class RPG_PROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	void EquipWeapon(USceneComponent* InParent, FName InSocketName);
protected:
	//UFUNCTION() needs to be commented as it is inherited from the parent class
	virtual void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	//VARIABLES
public:
	UPROPERTY(EditDefaultsOnly)
		ECharacterEquipState WeaponEquipState;
};
