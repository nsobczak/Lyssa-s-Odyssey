// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupKey.h"

// Sets default values for this actor's properties
APickupKey::APickupKey()
{
}

void APickupKey::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();
}

//report the power level of the battery
TEnumAsByte<KeyNature> APickupKey::GetKeyNature()
{
	return KeyNature;
}