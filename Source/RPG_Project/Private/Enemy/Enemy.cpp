// Fill out your copyright notice in the Description page of Project Settings.
#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "RPG_Project/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"

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

	const FVector Forward = GetActorForwardVector();
	//Equalize Z positions
	const FVector ImpactLower(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLower - GetActorLocation()).GetSafeNormal();
	
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//calculate inverse cosine of cos(theta) to calculate theta
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	//if cross p is pointing up, theta is positive, and ToHit is to the right of Forward
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	//Decide hit direction
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
		Section = FName("FromFront");
	else if (Theta >= 45.f && Theta < 135.f)
		Section = FName("FromRight");
	else if (Theta < -45.f && Theta >= -135.f)
		Section = FName("FromLeft");

	PlayHitReactMontage(Section);

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, 5.0, FColor::Blue, 5.f);

	UE_LOG(LogTemp, Warning, TEXT("Theta is : %f, Direction is : %s"), Theta, *Section.ToString());

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.0, FColor::Orange, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.0, FColor::Magenta, 5.f);

}