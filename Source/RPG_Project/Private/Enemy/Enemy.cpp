// Fill out your copyright notice in the Description page of Project Settings.
#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "RPG_Project/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidgetComponent)
		HealthBarWidgetComponent->SetHealthPercent(Attributes->GetHealthPercent());
}
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitImpact(ImpactPoint);
		if (HitSound)
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
		if (HitParticle)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ImpactPoint);
	}
	else
	{
		Die();
	}
}
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidgetComponent)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidgetComponent->SetHealthPercent(Attributes->GetHealthPercent());
	}
	return DamageAmount;
}
void AEnemy::Die()
{
	//Get suffix of end animation by getting a rand int between 1 and total number of sections
	int32 RandomSection = FMath::FRandRange(1, DeathMontage->GetNumSections());
	//Add the prefix
	FString SectionName("Death");
	//add the suffix
	SectionName.AppendInt(RandomSection);
	//Convert to FName and call the function
	PlayMontage(FName(SectionName), DeathMontage);
}
void AEnemy::DirectionalHitImpact(const FVector& ImpactPoint)
{
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
		Theta *= -1.f;
	//Decide hit direction
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
		Section = FName("FromFront");
	else if (Theta >= 45.f && Theta < 135.f)
		Section = FName("FromRight");
	else if (Theta < -45.f && Theta >= -135.f)
		Section = FName("FromLeft");

	PlayMontage(Section, HitReactMontage);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, 5.0, FColor::Blue, 5.f);
	//UE_LOG(LogTemp, Warning, TEXT("Theta is : %f, Direction is : %s"), Theta, *Section.ToString());
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.0, FColor::Orange, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.0, FColor::Magenta, 5.f);
}
void AEnemy::PlayMontage(const FName& SectionName, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr || Montage == nullptr) return;
	//play montage
	AnimInstance->Montage_Play(Montage);
	//play section of the montage
	AnimInstance->Montage_JumpToSection(SectionName, Montage);
	if (Montage == DeathMontage)
	{
		DeathSectionName = SectionName;
		FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveMontageInstance();
		if (MontageInstance)
		{
			MontageInstance->OnMontageBlendingOutStarted.BindUObject(this, &AEnemy::OnDeathMontageBlendingOut);
		}
	}
}

void AEnemy::OnDeathMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted && Montage == DeathMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		
		if (AnimInstance)
		{
				int32 SectionIndex = DeathMontage->GetSectionIndex(DeathSectionName);
				float SectionStart;
				float SectionEnd;
				DeathMontage->GetSectionStartAndEndTime(SectionIndex, SectionStart, SectionEnd);

				AnimInstance->Montage_Play(DeathMontage);
				AnimInstance->Montage_JumpToSection(DeathSectionName, DeathMontage);
				AnimInstance->Montage_SetPosition(
												DeathMontage,	
												SectionStart + DeathMontage->GetSectionLength(SectionIndex) - KINDA_SMALL_NUMBER);

				AnimInstance->Montage_SetPlayRate(DeathMontage, 0.0f);
		}
	}
}
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}