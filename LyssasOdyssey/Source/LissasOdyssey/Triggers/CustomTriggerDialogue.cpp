// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerDialogue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameModes/LevelGameMode.h"


// Sets default values
ACustomTriggerDialogue::ACustomTriggerDialogue()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomTriggerDialogue::BeginPlay()
{
	Super::BeginPlay();

	if (IsPlayerActorThatTriggers)
	{
		ActorThatTriggers = (AActor*)UGameplayStatics::GetPlayerPawn(this, 0);
	}
}

void ACustomTriggerDialogue::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	//triggered
	//UE_LOG(LogTemp, Warning, TEXT("dialogueTrigger"));

	// === GameMode ===
	ALevelGameMode* CurrentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (CurrentGameMode)
	{

	}
}

// Called every frame
void ACustomTriggerDialogue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
