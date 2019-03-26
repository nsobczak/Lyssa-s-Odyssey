// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerDialogue.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/LevelGameMode.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Utils/GameConstants.h"


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
	if (CurrentGameMode == nullptr) UE_LOG(LogTemp, Error, TEXT("CurrentGameMode is null"));


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


void ACustomTriggerDialogue::TypewriterEffect()
{
	//check if not already displayed entirely (if we clicked before end of recusivity)
	if (CurrentText.Equals(TargetText) || ToConsumedText.Len() <= 0) { return; }

	if (DEBUG)
		UE_LOG(LogTemp, Log, TEXT(" |ToConsumedText = %s|\n |ToConsumedText[0] = %c|\n |ToConsumedText.RightChop(1) = %s|\n |ToConsumedText = %s|"),
			*ToConsumedText, ToConsumedText[0], *ToConsumedText.RightChop(1), *ToConsumedText);

	CurrentText.AppendChar(ToConsumedText[0]);
	ToConsumedText = ToConsumedText.RightChop(1);
	CurrentGameMode->UpdateCurrentDialogueText(CurrentText);

	//timer not working when game pause on anything other than Actor => manually do timer in update
}


void ACustomTriggerDialogue::UpdateDialogueData()
{
	CurrentGameMode->UpdateDialoguePictureStruct(DialogueToDisplay[CurrentDialogueIndex].ProfilePicture);

	ELanguages currentLanguage = CurrentGameMode->CurrentLanguage;
	switch (currentLanguage)
	{
	case ELanguages::fr:
		TargetText = DialogueToDisplay[CurrentDialogueIndex].DialogueText.fr;
		break;

	default: //ELanguages::en
		TargetText = DialogueToDisplay[CurrentDialogueIndex].DialogueText.en;
		break;
	}

	//TODO: make following better
	if (TargetText.Contains(FString("<0>")))
	{
		int scorePickupAmountToTrigger = (int)(GameConstants::FINAL_GATE_SCORE_PERCENTAGE *
			(GameConstants::PICKUP_SCORE_MAX_CANYON + GameConstants::PICKUP_SCORE_MAX_FOREST +
				GameConstants::PICKUP_SCORE_MAX_ICE));
		int missingScore = scorePickupAmountToTrigger - Lyssa->GetPlayerPickupTotalScore();
		missingScore = missingScore < 0 ? 0 : missingScore;
		TargetText = TargetText.Replace(*FString("<0>"), *FString::FromInt(missingScore));
	}

	ToConsumedText = TargetText;
	CurrentText = FString("");
}


void ACustomTriggerDialogue::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	//triggered
	if (DEBUG)
		UE_LOG(LogTemp, Log, TEXT("dialogueTrigger"));

	if (DialogueToDisplay.Num() > 0 && CurrentGameMode)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		CurrentGameMode->ShowDialogueWidget(DisplayCursorWithDialogue);

		UpdateDialogueData();

		TypewriterEffect();
	}
}


// Called every frame
void ACustomTriggerDialogue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTriggered && CurrentGameMode)
	{
		//TypewriterEffect
		TextTimer += DeltaTime;
		if (TextTimer >= CurrentGameMode->TextSpeedFloatValue)
		{
			TypewriterEffect();
			TextTimer = 0;
		}

		//accept key pressed
		if (IsEventAccept)
		{
			if (!CurrentText.Equals(TargetText))
			{
				//display text entirely
				ToConsumedText = FString("");
				CurrentText = TargetText;
				CurrentGameMode->UpdateCurrentDialogueText(CurrentText);
			}

			else
			{
				//next text
				if (CurrentDialogueIndex < DialogueToDisplay.Num() - 1)
				{
					//show next string
					++CurrentDialogueIndex;
					UpdateDialogueData();
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
			}

			IsEventAccept = false;
		}
	}
}