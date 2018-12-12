// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Utils/RotateOverTime.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//All pickup start active
	bIsActive = true;

	URotateOverTime* rotationComponent = CreateDefaultSubobject<URotateOverTime>(TEXT("RotationOverTime"));
	AddOwnedComponent(rotationComponent);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APickup::IsActive()
{
	return bIsActive;
}

void APickup::SetActive(bool newPickupState)
{
	bIsActive = newPickupState;

	//Destroy if not active
	if (!bIsActive)
	{
		Destroy();
	}
}

void APickup::WasCollected_Implementation()
{
	FString pickupDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You collected %s"), *pickupDebugString);
}
