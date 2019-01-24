#include "MainGameMode.h"
#include "Core/Public/Math/UnrealMathUtility.h"
#include "Engine/Classes/Engine/World.h"
#include "Utils/GameConstants.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Public/SystemSettings.h"

#pragma region Initialization
//==============================================================================================
AMainGameMode::AMainGameMode()
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


bool AMainGameMode::ExecuteConsoleCommand(FString consoleCommand)
{
	if (GEngine)
	{
		UWorld* world = this->GetWorld();// GEngine->GetWorld();// GetEditorWorldContext().World();
		if (world)
		{
			return GEngine->Exec(world, *consoleCommand);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("world is null"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GEditor is null"));
		return false;
	}
}


#pragma region GameSave

void AMainGameMode::SaveSettingsValues(UMainSaveGame* SaveInstance)
{
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

void AMainGameMode::SaveGameSettings()
{
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
	// Windows .Sav File Paths:
		// If in editor:			\Unreal Projects\{UE4_PROJECT_NAME}\Saved\SaveGames\PlayerSaveSlot.sav
		// If in packaged game:		C:\Users\{YOUR_USERNAME}\AppData\Local\{UE4_PROJECT_NAME}\Saved\SaveGames\PlayerSaveSlot.sav
}

void AMainGameMode::LoadSettingsValues(UMainSaveGame * &LoadInstance)
{
	UE_LOG(LogTemp, Warning, TEXT("LoadSettingsValues function from mainGameMode"));

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
	this->EffectVolumeSliderValue = LoadInstance->EffectVolumeSliderValue;

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

void AMainGameMode::LoadGameSettings()
{
	UE_LOG(LogTemp, Warning, TEXT("LoadGameSettings function from AMainGameMode"));

	// Only load game stats if the load .sav file exists
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		class UMainSaveGame* LoadInstance = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

		if (LoadInstance->IsValidLowLevel())
		{
			LoadSettingsValues(LoadInstance);
		}
		else
		{
			// If save game object not found, create a new one
			class UMainSaveGame* LoadInstanceAlternate = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(
				UMainSaveGame::StaticClass()));

			if (!LoadInstanceAlternate)
				return;
			else
				LoadSettingsValues(LoadInstanceAlternate);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString(TEXT("No save game found.")), true);

		//TODO
		ALyssa* Lyssa = Cast<ALyssa>(UGameplayStatics::GetPlayerPawn(this, 0));
		if (Lyssa)
		{
			Lyssa->ResetTMapPlayerPickupAmountByLevel();
		}
		else
			UE_LOG(LogTemp, Error, TEXT("no result for GetPlayerPawn"));

		UseDefaultSettings();//InitializeKeySettingsWithDefault
	}
}
#pragma endregion


void AMainGameMode::UpdateAudioVolumes()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SCMusic, MusicVolumeSliderValue, 1.0f, 0, true);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SCEffect, EffectVolumeSliderValue, 1.0f, 0, true);
}


void AMainGameMode::InitializeTArrayAndApplyGraphicalSettings()
{
	//command
	if (TAGraphicalCommands.Num() < 1) TAGraphicalCommands.Append(GraphicalCommands, ARRAY_COUNT(GraphicalCommands));
	if (TAPPCommands.Num() < 1)TAPPCommands.Append(PPCommands, ARRAY_COUNT(PPCommands));
	if (TAAACommands.Num() < 1)TAAACommands.Append(AACommands, ARRAY_COUNT(AACommands));
	if (TAShadowCommands.Num() < 1)TAShadowCommands.Append(ShadowCommands, ARRAY_COUNT(ShadowCommands));
	if (TAFPSCommands.Num() < 1)TAFPSCommands.Append(FPSCommands, ARRAY_COUNT(FPSCommands));
	if (TAResCommands.Num() < 1)TAResCommands.Append(ResCommands, ARRAY_COUNT(ResCommands));

	ExecuteConsoleCommand(*(GraphicalCommands[GraphicalIndex]));
	ExecuteConsoleCommand(*(PPCommands[PPIndex]));
	ExecuteConsoleCommand(*(AACommands[AAIndex]));
	ExecuteConsoleCommand(*(ShadowCommands[ShadowIndex]));
	ExecuteConsoleCommand(*(FPSCommands[FPSIndex]));
	ExecuteConsoleCommand(*(ResCommands[ResIndex] + (IsFullScreen ? "f" : "w")));

	if (ShowFPS)
	{
		FString showFpsCommand = "stat fps";
		ExecuteConsoleCommand(*showFpsCommand);
	}
}


#pragma region Initialize default values
void AMainGameMode::InitializeGeneralSettingsWithDefault()
{
	this->CurrentLanguage = GameConstants::DefaultGeneral_CurrentLanguage;
}

void AMainGameMode::InitializeGraphicalSettingsWithDefault()
{
	this->ShowFPS = GameConstants::DefaultGraphical_ShowFPS;
	this->IsFullScreen = GameConstants::DefaultGraphical_IsFullScreen;
	this->GraphicalIndex = GameConstants::DefaultGraphical_GraphicalIndex;
	this->PPIndex = GameConstants::DefaultGraphical_PPIndex;
	this->AAIndex = GameConstants::DefaultGraphical_AAIndex;
	this->ShadowIndex = GameConstants::DefaultGraphical_ShadowIndex;
	this->FPSIndex = GameConstants::DefaultGraphical_FPSIndex;
	this->ResIndex = GameConstants::DefaultGraphical_ResIndex;
}

void AMainGameMode::InitializeAudioSettingsWithDefault()
{
	this->MasterVolumeSliderValue = GameConstants::DefaultAudio_MasterVolumeSliderValue;
	this->MusicVolumeSliderValue = GameConstants::DefaultAudio_MusicVolumeSliderValue;
	this->EffectVolumeSliderValue = GameConstants::DefaultAudio_EffectVolumeSliderValue;
}

void AMainGameMode::InitializeKeySettingsWithDefault()
{
	this->UseGamePad = GameConstants::UseGamePad;

	//GamePad
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::MoveUp, GameConstants::DefaultGPKey_MoveVertical);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::MoveRight, GameConstants::DefaultGPKey_MoveHorizontal);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::FMoveUp, GameConstants::DefaultGPKey_FMoveVertical);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::FMoveRight, GameConstants::DefaultGPKey_FMoveHorizontal);

	this->TMapGamepadKeys.Emplace(PlayerActionLabel::ACross, GameConstants::DefaultGPKey_ACross);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::ATriangle, GameConstants::DefaultGPKey_ATriangle);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::AStart, GameConstants::DefaultGPKey_AStart);

	//Keyboard
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::MoveUp, GameConstants::DefaultKKey_MoveUp);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::MoveDown, GameConstants::DefaultKKey_MoveDown);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::MoveLeft, GameConstants::DefaultKKey_MoveLeft);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::MoveRight, GameConstants::DefaultKKey_MoveRight);

	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::ACross, GameConstants::DefaultKKey_ACross);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::ATriangle, GameConstants::DefaultKKey_ATriangle);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::AStart, GameConstants::DefaultKKey_AStart);
}
#pragma endregion


void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->bAutoManageActiveCameraTarget = false;

	//load saved settings here
	LoadGameSettings();
	InitializeTArrayAndApplyGraphicalSettings();

	//Audio
	if (!SoundMix) UE_LOG(LogTemp, Error, TEXT("SoundMix is null."));
	if (!SCMusic) UE_LOG(LogTemp, Error, TEXT("SCMusic is null."));
	if (!SCEffect) UE_LOG(LogTemp, Error, TEXT("SCEffect is null."));
	UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
	//SetSoundMixClassOverride
	UpdateAudioVolumes();
	//play music
	if (MapTheme)
		UGameplayStatics::PlaySound2D(GetWorld(), MapTheme, 1.0f);

	//Menu
	if (IsMainMenu)
	{
		ChangeMenuWidget(StartingWidgetClass, true);
	}
}
//==============================================================================================
#pragma endregion

#pragma region Widgets
void AMainGameMode::ShowCursor(bool showCursor)
{
	//UE_LOG(LogTemp, Warning, TEXT("before if - showCursor = %s"), showCursor ? TEXT("true") : TEXT("false"));
	PlayerController->bShowMouseCursor = showCursor;
	PlayerController->bEnableClickEvents = showCursor;
	PlayerController->bEnableMouseOverEvents = showCursor;
}

void AMainGameMode::ShowWidget(TSubclassOf<UUserWidget> NewWidgetClass, bool showCursor)
{
	if (NewWidgetClass != nullptr)
	{
		OldWidget = CurrentWidget;

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}

		ShowCursor(showCursor);
	}
}

void AMainGameMode::HideWidget(UUserWidget* widgetToHide, bool showCursor)
{
	if (widgetToHide != nullptr)
	{
		widgetToHide->RemoveFromViewport();
	}

	ShowCursor(showCursor);
}

void AMainGameMode::HideCurrentWidget(bool showCursor)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	ShowCursor(showCursor);
}

void AMainGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, bool showCursor)
{
	OldWidget = CurrentWidget;

	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}

		ShowCursor(showCursor);
	}
}

void AMainGameMode::ShowStartingWidget()
{
	ChangeMenuWidget(StartingWidgetClass, true);
}

void AMainGameMode::ShowEndingWidget()
{
	ChangeMenuWidget(EndingWidgetClass, true);
}

void AMainGameMode::ShowGameOverWidget()
{
	ChangeMenuWidget(GameOverWidgetClass, true);
}

void AMainGameMode::ShowPauseWidget()
{
	ChangeMenuWidget(PauseWidgetClass, true);
}

void AMainGameMode::ShowHUD()
{
	ChangeMenuWidget(HUDWidget, false);
}


#pragma endregion

#pragma region Settings

void AMainGameMode::UseDefaultSettings()
{
	InitializeGeneralSettingsWithDefault();
	InitializeGraphicalSettingsWithDefault();
	InitializeAudioSettingsWithDefault();
	InitializeKeySettingsWithDefault();

	SaveGameSettings();
}

void AMainGameMode::ChangeCurrentLanguage(bool increase)
{
	uint8 maxIndexCurrentLanguage = (uint8)ELanguages::count;
	uint8 uint8CurrentLanguage = (uint8)this->CurrentLanguage;

	if (increase && uint8CurrentLanguage == maxIndexCurrentLanguage - 1)
		uint8CurrentLanguage = 0;
	else if (!increase && uint8CurrentLanguage == 0)
		uint8CurrentLanguage = maxIndexCurrentLanguage - 1;
	else
		increase ? ++uint8CurrentLanguage : --uint8CurrentLanguage;

	this->CurrentLanguage = (ELanguages)uint8CurrentLanguage;
}

void AMainGameMode::ChangeGraphicSetting(GraphicLabel graphicLabel, bool increase)
{
	TArray <FString> commandList;
	FString commandString;

	//index
	switch (graphicLabel)
	{
	case Graphical:
		increase ? ++GraphicalIndex : --GraphicalIndex;
		commandList = TAGraphicalCommands;
		GraphicalIndex = (GraphicalIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(GraphicalIndex < 0) ? 0 : GraphicalIndex;
		commandString = commandList[GraphicalIndex];
		break;

	case PP:
		increase ? ++PPIndex : --PPIndex;
		commandList = TAPPCommands;
		PPIndex = (PPIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(PPIndex < 0) ? 0 : PPIndex;
		commandString = commandList[PPIndex];
		break;

	case AA:
		increase ? ++AAIndex : --AAIndex;
		commandList = TAAACommands;
		AAIndex = (AAIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(AAIndex < 0) ? 0 : AAIndex;
		commandString = commandList[AAIndex];
		break;

	case Shadow:
		increase ? ++ShadowIndex : --ShadowIndex;
		commandList = TAShadowCommands;
		ShadowIndex = (ShadowIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(ShadowIndex < 0) ? 0 : ShadowIndex;
		commandString = commandList[ShadowIndex];
		break;

	case FPS:
		increase ? ++FPSIndex : --FPSIndex;
		commandList = TAFPSCommands;
		FPSIndex = (FPSIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(FPSIndex < 0) ? 0 : FPSIndex;
		commandString = commandList[FPSIndex];
		break;

	case Res:
		increase ? ++ResIndex : --ResIndex;
		commandList = TAResCommands;
		ResIndex = (ResIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(ResIndex < 0) ? 0 : ResIndex;
		commandString = commandList[ResIndex] + (IsFullScreen ? "f" : "w");
		break;

	default:
		break;
	}

	//command
	bool wasCommandExecuted = ExecuteConsoleCommand(*commandString);
	UE_LOG(LogTemp, Log, TEXT("exec command: %s, success?: %s"),
		*commandString, (wasCommandExecuted ? TEXT("True") : TEXT("False")));
}

bool AMainGameMode::SwitchUseGamePad()
{
	UseGamePad = !UseGamePad;
	return UseGamePad;
}

void AMainGameMode::AssignNewKey(FKey newKey, TEnumAsByte<PlayerActionLabel> actionToChange, bool isKeyboardKey)
{
	if (isKeyboardKey)
	{
		this->TMapKeyboardKeys.Emplace(actionToChange, newKey);
		//TODO: Check if key was already used, swap with old one if it was
	}

	else if (actionToChange != PlayerActionLabel::MoveDown && actionToChange != PlayerActionLabel::MoveLeft
		&& actionToChange != PlayerActionLabel::FMoveDown && actionToChange != PlayerActionLabel::FMoveLeft)
	{
		//Check if key is not already used, swap with old one if it is
		this->TMapGamepadKeys.Emplace(actionToChange, newKey);
	}
}

void AMainGameMode::ListenToNewKeyForMove(TEnumAsByte<PlayerActionLabel> actionToChange, int iteration)
{
	if (iteration >= 50) { IsListeningToKey = false; return; }

	IsListeningToKey = true;
	ListeningToKeyLabel = actionToChange;

	if (PlayerController->IsInputKeyDown(EKeys::AnyKey))//if any key pressed
	{
		//retrieve any key pressed
		FKey keyPressed;
		TArray<FKey> allKeys;
		EKeys::GetAllKeys(allKeys);
		for (size_t i = 0; i < allKeys.Num(); ++i)
		{
			if (allKeys[i] != EKeys::AnyKey && PlayerController->WasInputKeyJustPressed(allKeys[i]))
			{
				keyPressed = allKeys[i];
				FString keyPressedString = keyPressed.ToString();
				bool isAxis = EKeys::IsAxis(keyPressed);
				UE_LOG(LogTemp, Log, TEXT("key %s was pressed | is key Axis: %s"), *keyPressedString, (isAxis ? TEXT("True") : TEXT("False")));

				bool isKeyboardKey = !keyPressed.IsGamepadKey();
				if (!isKeyboardKey) UE_LOG(LogTemp, Log, TEXT("key %s is gamepadKey"), *keyPressedString);

				AssignNewKey(keyPressed, actionToChange, isKeyboardKey);
				IsListeningToKey = false;
				UE_LOG(LogTemp, Log, TEXT("IsListeningToKey = %s"), (IsListeningToKey ? TEXT("True") : TEXT("False")));

				return;
			}
		}
	}

	//delay
	FTimerHandle TimerHandle; // Handle to manage the timer
	FTimerDelegate TimerDel; //Bind function with parameters
	TimerDel.BindUFunction(this, FName("ListenToNewKeyForMove"), actionToChange, iteration + 1);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.1f, false);
}
#pragma endregion


void AMainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
