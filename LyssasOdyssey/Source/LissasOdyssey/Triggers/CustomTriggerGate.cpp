// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerGate.h"
#include "Utils/GameConstants.h"
#include "WorldAssets/Gate.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ACustomTriggerGate::ACustomTriggerGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

void ACustomTriggerGate::BeginPlay()
{
	Super::BeginPlay();

	if (IsPlayerActorThatTriggers)
		ActorThatTriggers = (AActor*)UGameplayStatics::GetPlayerPawn(this, 0);
}

void ACustomTriggerGate::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	if (!UseConditionToTriggerGates)
	{
		for (size_t i = 0; i < GatesToControl.Num(); i++)
		{
			AGate* currentGate = GatesToControl[i];
			if (currentGate)
				currentGate->SetIsOpen(ShouldTriggerOpen);
		}
	}
}