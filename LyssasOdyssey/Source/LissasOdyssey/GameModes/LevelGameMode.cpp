#include "LevelGameMode.h"
#include "GameModes/MainGameMode.h"
#include "Utils/GameConstants.h"
#include "WorldAssets/FinishArea.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Characters/Fylgja/Fylgja.h"
#include "Characters/Foes/Foe.h"
#include "Characters/CharacterActors/Shot.h"
#include "Pickups/PickupScore.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" //to be able to access the character and pause game
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"

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

	IsBeginFunctionCompleted = false;
}


#pragma region GameSave

void ALevelGameMode::SaveSettingsValues(UMainSaveGame* SaveInstance)
{
	//UE_LOG(LogTemp, Warning, TEXT("SaveSettingsValues function from levelGameMode"));

	Super::SaveSettingsValues(SaveInstance);

	//SaveInstance->CurrentLanguage = this->CurrentLanguage;

	//SaveInstance->GraphicalIndex = this->GraphicalIndex;
	//SaveInstance->PPIndex = this->PPIndex;
	//SaveInstance->AAIndex = this->AAIndex;
	//SaveInstance->ShadowIndex = this->ShadowIndex;
	//SaveInstance->FPSIndex = this->FPSIndex;
	//SaveInstance->ShowFPS = this->ShowFPS;
	//SaveInstance->ResolutionIndex = this->ResIndex;
	//SaveInstance->IsFullScreen = this->IsFullScreen;

	//SaveInstance->MasterVolumeSliderValue = this->MasterVolumeSliderValue;
	//SaveInstance->MusicVolumeSliderValue = this->MusicVolumeSliderValue;
	//SaveInstance->EffectVolumeSliderValue = this->EffectVolumeSliderValue;

	//SaveInstance->UseGamePad = this->UseGamePad;
	//SaveInstance->TMapKeyboardKeys = this->TMapKeyboardKeys;
	//SaveInstance->TMapGamepadKeys = this->TMapGamepadKeys;

	//// Save computer local date
	//SaveInstance->PlayerSaveSlotDate = FDateTime::Now();

	// Values
	SaveInstance->IsInitialized_TMapPlayerPickupByLevel = this->Lyssa->IsInitialized_TMapPlayerPickupByLevel;
	SaveInstance->TMapPlayerPickupByLevel = this->Lyssa->GetTMapPlayerPickupAmountByLevel();

	// Save to slot
	UGameplayStatics::SaveGameToSlot(SaveInstance, SaveSlotName, 0);

	UE_LOG(LogTemp, Log, TEXT("Game saved from ALevelGameMode"));
}

bool ALevelGameMode::SaveGameSettings()
{
	//UE_LOG(LogTemp, Warning, TEXT("SaveGameSettings function from levelGameMode"));

	// Create save game object, make sure it exists, then save player variables
	class UMainSaveGame* SaveInstance = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

	if (SaveInstance->IsValidLowLevel())
	{
		SaveSettingsValues(SaveInstance);
		return true;
	}
	else
	{
		// If the save game object is not found, create a new one
		class UMainSaveGame* SaveInstanceAlternate = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(
			UMainSaveGame::StaticClass()));

		if (!SaveInstanceAlternate)
			return false;
		else
		{
			SaveSettingsValues(SaveInstanceAlternate);
			return true;
		}
	}
}

void ALevelGameMode::LoadSettingsValues(UMainSaveGame * &LoadInstance)
{
	//UE_LOG(LogTemp, Warning, TEXT("LoadSettingsValues function from levelGameMode"));

	Super::LoadSettingsValues(LoadInstance);

	LoadInstance = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	Lyssa = Cast<ALyssa>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Lyssa)
	{
		this->Lyssa->IsInitialized_TMapPlayerPickupByLevel = LoadInstance->IsInitialized_TMapPlayerPickupByLevel;

		if (!LoadInstance->IsInitialized_TMapPlayerPickupByLevel)
		{
			//initialize tmap - save only if player finishes level
			Lyssa->ResetTMapPlayerPickupAmountByLevel();
		}
		else
		{
			//load existing tmap
			this->Lyssa->SetTMapPlayerPickupAmountByLevel(LoadInstance->TMapPlayerPickupByLevel);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Lyssa is null"));
	}

	UE_LOG(LogTemp, Log, TEXT("Game loaded from save!"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString(TEXT("Game loaded from save!")), true);
}

bool ALevelGameMode::LoadGameSettings()
{
	//UE_LOG(LogTemp, Warning, TEXT("LoadGameSettings function from levelGameMode"));

	// Only load game stats if the load .sav file exists
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		class UMainSaveGame* LoadInstance = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

		if (LoadInstance->IsValidLowLevel())
		{
			LoadSettingsValues(LoadInstance);
			return true;
		}
		else
		{
			// If save game object not found, create a new one
			class UMainSaveGame* LoadInstanceAlternate = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(
				UMainSaveGame::StaticClass()));

			if (!LoadInstanceAlternate)
				return false;
			else
				LoadSettingsValues(LoadInstanceAlternate);

			return true;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString(TEXT("No save game found.")), true);
		UE_LOG(LogTemp, Error, TEXT("No save game found. Main menu should be called first"));

		return false;
	}
}
#pragma endregion

#pragma region exec functions

#pragma region Pickup score functions
int ALevelGameMode::GetScorePickupCountInLevel()
{
	int result = 0;
	TArray<AActor*> pickups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupScore::StaticClass(), pickups);

	for (size_t i = 0; i < pickups.Num(); i++)
	{
		APickupScore* pickup = Cast<APickupScore>(pickups[i]);
		if (pickup)
			result++;
	}

	UE_LOG(LogTemp, Log, TEXT("There are %i score pickups in the level"), result);
	return result;
}

int ALevelGameMode::GetScorePickupValueInLevel()
{
	int result = 0;
	TArray<AActor*> pickups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupScore::StaticClass(), pickups);

	for (size_t i = 0; i < pickups.Num(); i++)
	{
		APickupScore* pickup = Cast<APickupScore>(pickups[i]);
		if (pickup)
			result += pickup->GetScoreAmount();
	}

	UE_LOG(LogTemp, Log, TEXT("All score pickups make a %i score in the level"), result);
	return result;
}

void ALevelGameMode::AddScorePickup(int amount)
{
	if (Lyssa)
		Lyssa->UpdateScorePickupAmount(amount);
}
#pragma endregion

#pragma region Key functions
bool ALevelGameMode::AddKey(TEnumAsByte<KeyLockNature> keyNature)
{
	bool result = false;
	if (Lyssa)
	{
		Lyssa->AddKeyPickup(keyNature);
		result = true;
	}

	UE_LOG(LogTemp, Log, TEXT("Manage to assign the key to lyssa? => %s"), (result ? TEXT("True") : TEXT("False")));
	return result;
}

bool ALevelGameMode::AddBlueKey()
{
	return AddKey(KeyLockNature::BLUE);
}

bool ALevelGameMode::AddRedKey()
{
	return AddKey(KeyLockNature::RED);
}

bool ALevelGameMode::AddYellowKey()
{
	return AddKey(KeyLockNature::YELLOW);
}

bool ALevelGameMode::AddGreenKey()
{
	return AddKey(KeyLockNature::GREEN);
}

bool ALevelGameMode::AddPurpleKey()
{
	return AddKey(KeyLockNature::PURPLE);
}


#pragma endregion

#pragma endregion

bool ALevelGameMode::GetIsBeginFunctionCompleted()
{
	return IsBeginFunctionCompleted;
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

	//UE_LOG(LogTemp, Log, TEXT("levelGameModed initialized lyssa variable"));
	IsBeginFunctionCompleted = true;
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
		Lyssa->DeathEffect();
		SetCurrentState(ELevelPlayState::EGameOver);
	}
}

void ALevelGameMode::ShowDialogueWidget(bool showCursor = true)
{
	if (nullptr != DialogueWidget)
	{
		//CurrentDialogueToDisplay = dialogue;
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

void ALevelGameMode::UpdateCurrentDialogueText(FString newDialogueText)
{
	CurrentDialogueText = newDialogueText;
}

void ALevelGameMode::UpdateDialoguePictureStruct(FStructTextureData profilePictureStruct)
{
	CurrentDialogueToDisplay.ProfilePicture = profilePictureStruct;
	//UE_LOG(LogTemp, Log, TEXT("updated profilePicture")); //CurrentDialogueText
}

void ALevelGameMode::UpdateDialogueSpeakerName(DialogueCharacterName dialogueCharacterName)
{
	CurrentDialogueToDisplay.DialogueCharacterName = dialogueCharacterName;
}


void ALevelGameMode::UpdateDialogueTextStruct(FStructTextTrad dialogueTextStruct)
{
	CurrentDialogueToDisplay.DialogueText = dialogueTextStruct;
	//UE_LOG(LogTemp, Log, TEXT("updated dialogueTextStruct"));
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
		OnGameOver();

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
		OnLevelCompleted();

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

		//pickup amount for this level
		if (Lyssa->GetTMapPlayerPickupAmountByLevel().Contains(this->LevelLabel)
			&& Lyssa->GetCurrentScorePickupAmount() > Lyssa->GetTMapPlayerPickupAmountByLevel().FindRef(this->LevelLabel))
		{
			Lyssa->EmplaceTMapPlayerPickupAmountByLevel(this->LevelLabel, Lyssa->GetCurrentScorePickupAmount());

			this->SaveGameSettings();
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

