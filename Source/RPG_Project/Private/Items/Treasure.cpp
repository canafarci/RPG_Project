// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Characters/RPGCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARPGCharacter* Character = Cast<ARPGCharacter>(OtherActor);
	if (Character == nullptr) return;
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	Destroy();
}
