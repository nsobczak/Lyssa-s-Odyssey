// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerFoe.h"
#include "Characters/Foes/Foe.h"
#include "Characters/Foes/Boss.h"
#include "GameModes/LevelGameMode.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "UI/HUD_BossInfo.h"

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

	this->LevelGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!LevelGameMode)
		UE_LOG(LogTemp, Error, TEXT("LevelGameMode is null"));
}

void ACustomTriggerFoe::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	for (size_t i = 0; i < FoesToControl.Num(); i++)
	{
		AFoe* currentFoe = FoesToControl[i];
		if (currentFoe)
		{
			currentFoe->SetIsFoeActive(ShouldActivateFoes);

			ABoss* currentBoss = Cast<ABoss>(currentFoe);
			if (currentBoss)
			{
				this->LevelGameMode->HudBossInfo->SetCurrentBoss(currentBoss);
				this->LevelGameMode->HudBossInfo->ShowInfo();
			}
		}
	}
}