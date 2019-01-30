// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerFoe.h"
#include "Characters/Foes/Foe.h"

// Sets default values
ACustomTriggerFoe::ACustomTriggerFoe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

void ACustomTriggerFoe::BeginPlay()
{
	Super::BeginPlay();

	if (IsPlayerActorThatTriggers)
		ActorThatTriggers = (AActor*)UGameplayStatics::GetPlayerPawn(this, 0);
}

void ACustomTriggerFoe::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	for (size_t i = 0; i < FoesToControl.Num(); i++)
	{
		AFoe* currentFoe = FoesToControl[i];
		if (currentFoe)
			currentFoe->SetIsFoeActive(ShouldActivateFoes);
	}
}