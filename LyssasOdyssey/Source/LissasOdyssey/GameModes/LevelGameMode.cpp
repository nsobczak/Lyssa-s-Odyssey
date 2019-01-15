// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGameMode.h"
#include "GameModes/MainGameMode.h"
#include "WorldAssets/FinishArea.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Characters/Fylgja/Fylgja.h"
#include "Characters/Foes/Foe.h"
#include "Characters/CharacterActors/Shot.h"
#include "Pickups/PickupScore.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" //to be able to access the character and pause game
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"

//#include "Engine/DataTable.h"

// Sets default values
ALevelGameMode::ALevelGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ALyssa::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("/Game/Characters/Lyssa/BPLyssa"));
	if (PlayerPawnObject.Succeeded())
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}

	//HUDClass = AFPSHUD::StaticClass();
}


// Called when the game starts or when spawned
void ALevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(ELevelPlayState::EPlaying);

	//hud
	if (nullptr != HUDClass)
	{
		ShowHUD();
	}

	LevelTimer = 0;
	bPauseable = true;

	// === playerController ===
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (playerController)
	{
		playerController->SetInputMode(FInputModeGameAndUI());
	}

	if (ShowMouseCursorInLevel && !UseGamePad)
	{
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->bShowMouseCursor = true;
	}

	// === Lyssa + fylgja ===
	Lyssa = Cast<ALyssa>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Lyssa)
	{
		TArray<AActor*> children;
		Lyssa->GetAllChildActors(children, true);
		for (size_t i = 0; i < children.Num(); ++i)
		{
			AFylgja* tmpFylgja = Cast<AFylgja>(children[i]);
			if (tmpFylgja)
			{
				Fylgja = tmpFylgja;
				if (ShouldFylgjaStartVisible)
					Fylgja->ShowFylfja();
				else
					Fylgja->HideFylfja();
			}
		}
	}

	// === LevelTotalScore === 
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupScore::StaticClass(), FoundActors);
	LevelTotalScore = 0;
	for (size_t i = 0; i < FoundActors.Num(); i++)
	{
		APickupScore* currentPickupScore = (APickupScore*)FoundActors[i];
		if (currentPickupScore)
			LevelTotalScore += currentPickupScore->GetScoreAmount();
	}
}


//_____________________________________________________________________________________________

FText ALevelGameMode::GetTimerForHud()
{
	//timer is in second
	int seconds = (int)LevelTimer;
	int minutes = seconds / 60;
	int hours = minutes / 60;

	seconds %= 60;
	minutes %= 60;

	FString fSeconds = FString::FromInt(seconds) + FString("\"");
	FString fMinutes = FString::FromInt(minutes) + FString("\'");
	FString fHours = FString::FromInt(hours) + FString(":");

	FString textToShow = hours > 0 ? fHours + fMinutes + fSeconds : fMinutes + fSeconds;
	return FText::FromString(textToShow);
}

void ALevelGameMode::CheckForDeath()
{
	if (Lyssa->GetCurrentLife() < 0.0f)
	{
		SetCurrentState(ELevelPlayState::EGameOver);
	}
}

void ALevelGameMode::ShowDialogueWidget(FStructDialogue dialogue, bool showCursor = true)
{
	if (nullptr != DialogueWidget)
	{
		CurrentDialogueToDisplay = dialogue;
		//change visibility of DialogueWidget
		DialogueWidget->SetVisibility(ESlateVisibility::Visible);
		ShowCursor(showCursor);

		IsPauseAllowed = false;
	}

}

void ALevelGameMode::HideDialogueWidget(bool showCursor)
{
	if (nullptr != DialogueWidget)
	{
		//change visibility of DialogueWidget
		DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
		ShowCursor(showCursor);

		IsPauseAllowed = true;
	}
}

void ALevelGameMode::UpdateDialogue(FStructDialogue dialogue)
{
	CurrentDialogueToDisplay = dialogue;
	//UE_LOG(LogTemp, Log, TEXT("updated dialogue"));
}


//_____________________________________________________________________________________________

// Called every frame
void ALevelGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LevelTimer += DeltaTime;

	DamageRateTimer -= DeltaTime;
	if (DamageRateTimer < DamageRate)
	{
		//HandleProjectileDamage();
		DamageRateTimer = DamageRate;
	}

	if (Currentstate == ELevelPlayState::EPlaying)
	{
		CheckForDeath();
	}
}


#pragma region PlayStates

ELevelPlayState ALevelGameMode::GetCurrentState() const
{
	return Currentstate;
}

void ALevelGameMode::SetCurrentState(ELevelPlayState newState)
{
	Currentstate = newState;
	HandleNewState(Currentstate);
}

void ALevelGameMode::HandleNewState(ELevelPlayState newState)
{
	switch (newState)
	{
	case ELevelPlayState::EPlaying:
	{
		//resume pause
		if (UGameplayStatics::IsGamePaused(GetWorld()))
		{
			ShowHUD();
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}

		break;
	}

	case ELevelPlayState::EPause:
	{
		//UE_LOG(LogTemp, Warning, TEXT("Game paused"));
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		ShowPauseWidget();

		break;
	}

	case ELevelPlayState::EGameOver:
	{
		UE_LOG(LogTemp, Log, TEXT("Lyssa died | game over."));

		AMainGameMode* gameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			//block player input
			APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
			if (playerController)
			{
				playerController->SetInputMode(FInputModeUIOnly());
				playerController->SetCinematicMode(true, false, false, true, true);
			}
			gameMode->ShowGameOverWidget();
		}

		break;
	}

	case ELevelPlayState::ELevelCompleted:
	{
		UE_LOG(LogTemp, Log, TEXT("Level Completed!"));

		AMainGameMode* gameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			//block player input
			APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
			if (playerController)
			{
				playerController->SetInputMode(FInputModeUIOnly());
				playerController->SetCinematicMode(true, false, false, true, true);
			}
			gameMode->ShowEndingWidget();
		}

		break;
	}

	case ELevelPlayState::EUnknown:
		//do nothing
		break;

	default:
		//do nothing
		break;
	}
}

#pragma endregion

