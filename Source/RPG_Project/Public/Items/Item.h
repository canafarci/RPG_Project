// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class RPG_PROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintPure)
	float TransformSine();

	template<typename T>
	T Avg(T First, T Second);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
		float Amplitude = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime = 0.f;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
