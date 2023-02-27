// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/RPGCharacter.h"

void AWeapon::EquipWeapon(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
	ItemMesh->AttachToComponent(InParent, AttachmentRule, InSocketName);
}

void AWeapon::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnItemBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnItemEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
