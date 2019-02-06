// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerGateWithCondition.h"
#include "Characters/Lyssa/Lyssa.h"
#include "WorldAssets/Gate.h"

// Sets default values
ACustomTriggerGateWithCondition::ACustomTriggerGateWithCondition()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

void ACustomTriggerGateWithCondition::BeginPlay()
{
	Super::BeginPlay();

	UseConditionToTriggerGates = true;
}

void ACustomTriggerGateWithCondition::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	ALyssa* lyssa = Cast<ALyssa>(ActorThatTriggers);
	if (lyssa)
	{
		int playerPickupTotalScore = lyssa->GetPlayerPickupTotalScore();
		if (playerPickupTotalScore >= ScorePickupAmountToTrigger)
		{
			UE_LOG(LogTemp, Log, TEXT("player has enough score pickups, he needs %i to trigger gates and he has %i"),
				ScorePickupAmountToTrigger, playerPickupTotalScore);
			for (size_t i = 0; i < GatesToControl.Num(); i++)
			{
				AGate* currentGate = GatesToControl[i];
				if (currentGate)
					currentGate->SetIsOpen(ShouldTriggerOpen);
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("player doesn't have enough score pickups, he needs %i to trigger gates and he has %i"),
				ScorePickupAmountToTrigger, playerPickupTotalScore);
		}
	}
}