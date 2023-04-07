// Fill out your copyright notice in the Description page of Project Settings.
#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "RPG_Project/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}
void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr || HitReactMontage == nullptr) return;
	//play montage
	AnimInstance->Montage_Play(HitReactMontage);
	//play section of the montage
	AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
}
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE(ImpactPoint, 10.f);
	PlayHitReactMontage(FName("FromLeft"));
}