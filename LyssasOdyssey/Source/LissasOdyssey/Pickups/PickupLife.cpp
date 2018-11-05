// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupLife.h"

// Sets default values for this actor's properties
APickupLife::APickupLife()
{
}

void APickupLife::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();
}

//report the power level of the battery
float APickupLife::GetLifeAmount()
{
	return LifeAmount;
}



