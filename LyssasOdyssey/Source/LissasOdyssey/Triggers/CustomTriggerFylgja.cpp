// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerFylgja.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameModes/LevelGameMode.h"
#include "Characters/Fylgja/Fylgja.h"

// Sets default values
ACustomTriggerFylgja::ACustomTriggerFylgja()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustomTriggerFylgja::BeginPlay()
{
	Super::BeginPlay();

	if (IsPlayerActorThatTriggers)
	{
		ActorThatTriggers = (AActor*)UGameplayStatics::GetPlayerPawn(this, 0);
	}
}

void ACustomTriggerFylgja::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	// === GameMode ===
	ALevelGameMode* currentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (currentGameMode && currentGameMode->Fylgja)
	{
		if (TriggerFylgja)
			currentGameMode->Fylgja->ShowFylfja();
		else
			currentGameMode->Fylgja->HideFylfja();
	}
}

//// Called every frame
//void ACustomTriggerFylgja::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	if (IsTriggered)
//	{
//
//	}
//}