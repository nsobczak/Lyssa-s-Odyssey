// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerDialogue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/LevelGameMode.h"


// Sets default values
ACustomTriggerDialogue::ACustomTriggerDialogue()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

}

// Called when the game starts or when spawned
void ACustomTriggerDialogue::BeginPlay()
{
	Super::BeginPlay();

	if (IsPlayerActorThatTriggers)
	{
		ActorThatTriggers = (AActor*)UGameplayStatics::GetPlayerPawn(this, 0);
	}

	CurrentDialogueIndex = 0;
}

void ACustomTriggerDialogue::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	//triggered
	//UE_LOG(LogTemp, Log, TEXT("dialogueTrigger"));

	// === GameMode ===
	ALevelGameMode* CurrentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (DialogueToDisplay.Num() > 0 && CurrentGameMode)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		CurrentGameMode->ShowDialogueWidget(DialogueToDisplay[CurrentDialogueIndex], DisplayCursorWithDialogue);
	}
}

// Called every frame
void ACustomTriggerDialogue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTriggered)
	{
		ALevelGameMode* CurrentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
		if (CurrentGameMode && CurrentGameMode->PlayerController->WasInputKeyJustReleased(CurrentGameMode->KeyList[4]))//if any key pressed
		{
			if (CurrentDialogueIndex < DialogueToDisplay.Num() - 1)
			{
				//show next string
				++CurrentDialogueIndex;
				CurrentGameMode->UpdateDialogue(DialogueToDisplay[CurrentDialogueIndex]);
			}
			else
			{
				//set widget visibility to hidden
				CurrentGameMode->HideDialogueWidget();
				UGameplayStatics::SetGamePaused(GetWorld(), false);
			}
		}
	}
}