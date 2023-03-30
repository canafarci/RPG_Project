// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "RPG_PROJECT/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/RPGCharacter.h"
// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere= CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	Sphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnItemBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnItemEndOverlap);
}

float AItem::TransformSine()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

void AItem::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("BEGIN OVERLAP  \n this: %s, other: %s"), *OverlappedComponent->GetName(), *OtherActor->GetName());
	ARPGCharacter* Character = Cast<ARPGCharacter>(OtherActor);
	if (Character == nullptr) return;
	Character->SetOverlappingItem(this);

}

void AItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("END OVERLAP \n this: %s other: %s"), *OverlappedComponent->GetName(), *OtherComp->GetName())
	ARPGCharacter* Character = Cast<ARPGCharacter>(OtherActor);
	if (Character == nullptr) return;
	Character->SetOverlappingItem(nullptr);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	if (ItemState != EItemState::EIS_Hovering) return;
	AddActorWorldOffset(FVector(0.f, 0.f, TransformSine()));
}