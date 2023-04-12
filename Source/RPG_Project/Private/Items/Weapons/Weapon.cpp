// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/RPGCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	//initialize actors to ignore list for linetracing
	ActorsToIgnore.Add(this);
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}
void AWeapon::EquipWeapon(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;

	UWorld* World = GetWorld();
	if (World && SoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(World, SoundCue, GetActorLocation());
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Sphere->SetGenerateOverlapEvents(false);
	}
	if (ItemEffect)
		ItemEffect->Deactivate();
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AttachmentRule, InSocketName);
}

void AWeapon::ToggleHitCollision(bool bEnable)
{
	if (bEnable)
		WeaponBox->SetGenerateOverlapEvents(true);
	else
	{
		WeaponBox->SetGenerateOverlapEvents(false);
		//reset ignored actors list
		ActorsToIgnore.Empty();
		ActorsToIgnore.Add(this);
	}
}

void AWeapon::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnItemBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnItemEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	FHitResult OutHit;
	UKismetSystemLibrary::BoxTraceSingle(this,
										Start,
										End,
										FVector(5.f, 5.f, 5.f),
										BoxTraceStart->GetComponentRotation(),
										ETraceTypeQuery::TraceTypeQuery1,
										false,
										ActorsToIgnore,
										EDrawDebugTrace::None,
										OutHit,
										true);
	if (OutHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(OutHit.GetActor());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(OutHit.GetActor(), OutHit.ImpactPoint);
		}
		ActorsToIgnore.AddUnique(OutHit.GetActor());

		if (OutHit.Component != nullptr &&
			OutHit.Component->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
		{
			CreateFields(OutHit.ImpactPoint);
		}
	}
}
