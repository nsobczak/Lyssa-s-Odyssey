// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerGateWithCondition.h"
#include "Characters/Lyssa/Lyssa.h"
#include "WorldAssets/Gate.h"
#include "Utils/GameConstants.h"

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

	if (IsFinalGate)
	{
		ScorePickupAmountToTrigger = (int)(TotalPickupAmountPercentage *
			(GameConstants::PICKUP_SCORE_MAX_CANYON + GameConstants::PICKUP_SCORE_MAX_FOREST +
				GameConstants::PICKUP_SCORE_MAX_ICE));
	}
}

void ACustomTriggerGateWithCondition::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	ALyssa* lyssa = Cast<ALyssa>(ActorThatTriggers);
	if (lyssa)
	{
		int playerPickupTotalScore = TriggerWithTotalScoreInsteadOfCurrentScore ?
			lyssa->GetPlayerPickupTotalScore() : lyssa->GetCurrentScorePickupAmount();
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