// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupScore.h"



// Sets default values for this actor's properties
APickupScore::APickupScore()
{
}

void APickupScore::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();
}

//report the power level of the battery
int APickupScore::GetScoreAmount()
{
	return ScoreAmount;
}



