// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "RPG_Project/Enums.h"
#include "Weapon.generated.h"

/**
 * 
 */

class USoundBase;
class UBoxComponent;

UCLASS()
class RPG_PROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void EquipWeapon(USceneComponent* InParent, FName InSocketName);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	void ToggleHitCollision(bool bEnable);
protected:
	virtual void BeginPlay() override;
	//UFUNCTION() needs to be commented as it is inherited from the parent class
	virtual void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
public:
	UPROPERTY(EditDefaultsOnly)
	ECharacterEquipState WeaponEquipState;
private:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* SoundCue;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* BoxTraceEnd;
	TArray<AActor*> ActorsToIgnore;

};
