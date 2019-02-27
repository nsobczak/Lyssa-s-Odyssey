// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerDialogue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/LevelGameMode.h"
#include "Characters/Lyssa/Lyssa.h"


// Sets default values
ACustomTriggerDialogue::ACustomTriggerDialogue()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

}

void ACustomTriggerDialogue::OnActionAccept()
{
	if (IsTriggered)
	{
		IsEventAccept = true;
	}
}

// Called when the game starts or when spawned
void ACustomTriggerDialogue::BeginPlay()
{
	Super::BeginPlay();

	Lyssa = (ALyssa*)UGameplayStatics::GetPlayerPawn(this, 0);
	CurrentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();

	if (IsPlayerActorThatTriggers)
	{
		ActorThatTriggers = (AActor*)Lyssa;//UGameplayStatics::GetPlayerPawn(this, 0)
	}

	//delegates
	Lyssa->OnAcceptDelegate.AddDynamic(this, &ACustomTriggerDialogue::OnActionAccept);

	DialogueToDisplay.Empty();
	if (nullptr != DialogueDataTable && DialogueRows.Num() > 0)
	{
		for (size_t i = 0; i < DialogueRows.Num(); ++i)
		{
			FStructDialogue* dialogueItem = DialogueDataTable->FindRow<FStructDialogue>(DialogueRows[i], "dialogueItem", true);
			if (dialogueItem)
			{
				DialogueToDisplay.Add(*dialogueItem);
			}
		}
	}

	CurrentDialogueIndex = 0;
}

void ACustomTriggerDialogue::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	//triggered
	//UE_LOG(LogTemp, Log, TEXT("dialogueTrigger"));

	// === GameMode ===
	if (DialogueToDisplay.Num() > 0 && CurrentGameMode)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		CurrentGameMode->ShowDialogueWidget(DisplayCursorWithDialogue);//DialogueToDisplay[CurrentDialogueIndex]
	}
}



// Called every frame
void ACustomTriggerDialogue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTriggered)
	{
		//ALevelGameMode* currentLGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();

		//typewriter effect
		//CurrentGameMode->UpdateDialogue

		if (CurrentGameMode && IsEventAccept)//if any key pressed
		{
			if (CurrentDialogueIndex < DialogueToDisplay.Num() - 1)
			{
				//show next string
				++CurrentDialogueIndex;
				CurrentGameMode->UpdateDialogue(DialogueToDisplay[CurrentDialogueIndex]);
			}
			else
			{
				//reset
				CurrentDialogueIndex = 0;
				IsTriggered = false;

				//set widget visibility to hidden
				CurrentGameMode->HideDialogueWidget();
				UGameplayStatics::SetGamePaused(GetWorld(), false);
			}

			IsEventAccept = false;
		}
	}
}