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

#pragma region Initialization
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


#pragma region GameSave

void ALevelGameMode::SaveSettingsValues(UMainSaveGame* SaveInstance)
{
	UE_LOG(LogTemp, Warning, TEXT("SaveSettingsValues function from levelGameMode"));

	// Values
	SaveInstance->CurrentLanguage = this->CurrentLanguage;

	SaveInstance->GraphicalIndex = this->GraphicalIndex;
	SaveInstance->PPIndex = this->PPIndex;
	SaveInstance->AAIndex = this->AAIndex;
	SaveInstance->ShadowIndex = this->ShadowIndex;
	SaveInstance->FPSIndex = this->FPSIndex;
	SaveInstance->ShowFPS = this->ShowFPS;
	SaveInstance->ResolutionIndex = this->ResIndex;
	SaveInstance->IsFullScreen = this->IsFullScreen;

	SaveInstance->MasterVolumeSliderValue = this->MasterVolumeSliderValue;
	SaveInstance->MusicVolumeSliderValue = this->MusicVolumeSliderValue;
	SaveInstance->EffectVolumeSliderValue = this->EffectVolumeSliderValue;

	SaveInstance->UseGamePad = this->UseGamePad;
	SaveInstance->TMapKeyboardKeys = this->TMapKeyboardKeys;
	SaveInstance->TMapGamepadKeys = this->TMapGamepadKeys;

	// Save computer local date
	SaveInstance->PlayerSaveSlotDate = FDateTime::Now();

	// Save to slot
	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveSlotName, 0);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString(TEXT("Game saved!")), true);
}

void ALevelGameMode::SaveGameSettings()
{
	Super::SaveGameSettings();

	UE_LOG(LogTemp, Warning, TEXT("SaveGameSettings function from levelGameMode"));

	// Create save game object, make sure it exists, then save player variables
	class UMainSaveGame* SaveInstance = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

	if (SaveInstance->IsValidLowLevel())
	{
		SaveSettingsValues(SaveInstance);
	}
	else
	{
		// If the save game object is not found, create a new one
		class UMainSaveGame* SaveInstanceAlternate = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(
			UMainSaveGame::StaticClass()));

		if (!SaveInstanceAlternate)
			return;
		else
			SaveSettingsValues(SaveInstanceAlternate);
	}
}

void ALevelGameMode::LoadSettingsValues(UMainSaveGame * &LoadInstance)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadSettingsValues function from levelGameMode"));

	LoadInstance = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	this->CurrentLanguage = LoadInstance->CurrentLanguage;

	this->GraphicalIndex = LoadInstance->GraphicalIndex;
	this->PPIndex = LoadInstance->PPIndex;
	this->AAIndex = LoadInstance->AAIndex;
	this->ShadowIndex = LoadInstance->ShadowIndex;
	this->FPSIndex = LoadInstance->FPSIndex;
	this->ShowFPS = LoadInstance->ShowFPS;
	this->ResIndex = LoadInstance->ResolutionIndex;
	this->IsFullScreen = LoadInstance->IsFullScreen;

	this->MasterVolumeSliderValue = LoadInstance->MasterVolumeSliderValue;
	this->MusicVolumeSliderValue = LoadInstance->MusicVolumeSliderValue;

	this->UseGamePad = LoadInstance->UseGamePad;
	this->TMapKeyboardKeys = LoadInstance->TMapKeyboardKeys;
	this->TMapGamepadKeys = LoadInstance->TMapGamepadKeys;

	ALyssa* player = (ALyssa*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (nullptr != player)
	{
		UE_LOG(LogTemp, Log, TEXT("player is %s | player->InputComponent = %s"), *(player->GetName()), *(player->InputComponent->GetFName().ToString()));
		player->SetupPlayerInputComponent(player->InputComponent);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("no player pawn detected in level"));
	}

	UE_LOG(LogTemp, Log, TEXT("Game loaded from save!"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString(TEXT("Game loaded from save!")), true);
}

void ALevelGameMode::LoadGameSettings()
{
	Super::LoadGameSettings();

	UE_LOG(LogTemp, Warning, TEXT("LoadGameSettings function from levelGameMode"));

	//// Only load game stats if the load .sav file exists
	//if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	//{
	//	class UMainSaveGame* LoadInstance = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

	//	if (LoadInstance->IsValidLowLevel())
	//	{
	//		LoadSettingsValues(LoadInstance);
	//	}
	//	else
	//	{
	//		// If save game object not found, create a new one
	//		class UMainSaveGame* LoadInstanceAlternate = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(
	//			UMainSaveGame::StaticClass()));

	//		if (!LoadInstanceAlternate)
	//			return;
	//		else
	//			LoadSettingsValues(LoadInstanceAlternate);
	//	}
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString(TEXT("No save game found.")), true);
	//	InitializeKeySettings();

	//	ALyssa* Lyssa = Cast<ALyssa>(UGameplayStatics::GetPlayerPawn(this, 0));
	//	if (Lyssa)
	//	{
	//		Lyssa->ResetTMapPlayerPickupAmountByLevel();
	//	}
	//	else
	//		UE_LOG(LogTemp, Error, TEXT("no result for GetPlayerPawn"));
	//}
}
#pragma endregion


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

	//// === LevelTotalScore === 
	//TArray<AActor*> FoundActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupScore::StaticClass(), FoundActors);
	//LevelTotalScore = 0;
	//for (size_t i = 0; i < FoundActors.Num(); i++)
	//{
	//	APickupScore* currentPickupScore = (APickupScore*)FoundActors[i];
	//	if (currentPickupScore)
	//		LevelTotalScore += currentPickupScore->GetScoreAmount();
	//}
}

#pragma endregion
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

