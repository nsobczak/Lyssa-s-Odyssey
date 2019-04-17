#include "MainGameMode.h"

#include "Core/Public/Math/UnrealMathUtility.h"
#include "Engine/Classes/Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Public/SystemSettings.h"

#include "Utils/GameConstants.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Characters/CharacterWithInputs.h"
#include "UI/ButtonBase.h"


#pragma region Initialization
//==============================================================================================
AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ACharacterWithInputs::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("/Game/Characters/BPDefaultCharacterWithInputs"));
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
	SaveInstance->CurrentTextSpeed = this->CurrentTextSpeed;

	SaveInstance->LyssaBodyMatIdx = this->LyssaBodyMatIdx;
	SaveInstance->LyssaShapeMatIdx = this->LyssaShapeMatIdx;

	SaveInstance->ShowMinimap = this->ShowMap;
	SaveInstance->ShowTime = this->ShowTime;
	SaveInstance->ShowLife = this->ShowLife;
	SaveInstance->ShowTitle = this->ShowTitle;
	SaveInstance->ShowPickup = this->ShowPickup;
	SaveInstance->ShowMouseCursorInLevel = this->ShowMouseCursorInLevel;

	SaveInstance->GraphicalIndex = this->GraphicalIndex;
	SaveInstance->PPIndex = this->PPIndex;
	SaveInstance->AAIndex = this->AAIndex;
	SaveInstance->ShadowIndex = this->ShadowIndex;
	SaveInstance->FPSIndex = this->FPSIndex;
	//SaveInstance->ShowFPS = this->ShowFPS;
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

	UE_LOG(LogTemp, Log, TEXT("Game saved from AMainGameMode"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString(TEXT("Game saved!")), true);
}

bool AMainGameMode::SaveGameSettings()
{
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
	// Windows .Sav File Paths:
		// If in editor:			\Unreal Projects\{UE4_PROJECT_NAME}\Saved\SaveGames\PlayerSaveSlot.sav
		// If in packaged game:		C:\Users\{YOUR_USERNAME}\AppData\Local\{UE4_PROJECT_NAME}\Saved\SaveGames\PlayerSaveSlot.sav
}

void AMainGameMode::LoadSettingsValues(UMainSaveGame * &LoadInstance)
{
	//UE_LOG(LogTemp, Warning, TEXT("LoadSettingsValues function from mainGameMode"));

	LoadInstance = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	this->CurrentLanguage = LoadInstance->CurrentLanguage;
	this->CurrentTextSpeed = LoadInstance->CurrentTextSpeed;
	UpdateTextSpeedFloatValue();

	this->LyssaBodyMatIdx = LoadInstance->LyssaBodyMatIdx;
	this->LyssaShapeMatIdx = LoadInstance->LyssaShapeMatIdx;

	this->ShowMap = LoadInstance->ShowMinimap;
	this->ShowTime = LoadInstance->ShowTime;
	this->ShowLife = LoadInstance->ShowLife;
	this->ShowTitle = LoadInstance->ShowTitle;
	this->ShowPickup = LoadInstance->ShowPickup;
	this->ShowMouseCursorInLevel = LoadInstance->ShowMouseCursorInLevel;

	this->GraphicalIndex = LoadInstance->GraphicalIndex;
	this->PPIndex = LoadInstance->PPIndex;
	this->AAIndex = LoadInstance->AAIndex;
	this->ShadowIndex = LoadInstance->ShadowIndex;
	this->FPSIndex = LoadInstance->FPSIndex;
	//this->ShowFPS = LoadInstance->ShowFPS;
	this->ResIndex = LoadInstance->ResolutionIndex;
	this->IsFullScreen = LoadInstance->IsFullScreen;

	this->MasterVolumeSliderValue = LoadInstance->MasterVolumeSliderValue;
	this->MusicVolumeSliderValue = LoadInstance->MusicVolumeSliderValue;
	this->EffectVolumeSliderValue = LoadInstance->EffectVolumeSliderValue;

	this->UseGamePad = LoadInstance->UseGamePad;
	this->TMapKeyboardKeys = LoadInstance->TMapKeyboardKeys;
	this->TMapGamepadKeys = LoadInstance->TMapGamepadKeys;

	UE_LOG(LogTemp, Log, TEXT("Game loaded from save!"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString(TEXT("Game loaded from save!")), true);
}

bool AMainGameMode::LoadGameSettings()
{
	//UE_LOG(LogTemp, Warning, TEXT("LoadGameSettings function from AMainGameMode"));

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
			this->DoesSaveGameExist = false;
			// If save game object not found, create a new one
			class UMainSaveGame* LoadInstanceAlternate = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(
				UMainSaveGame::StaticClass()));

			if (!LoadInstanceAlternate)
				return false;
			else
				LoadSettingsValues(LoadInstanceAlternate);
		}
		this->DoesSaveGameExist = true;
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString(TEXT("No save game found.")), true);
		this->DoesSaveGameExist = false;

		UseDefaultSettings();
		return false;
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
}


#pragma region Initialize default values
void AMainGameMode::InitializeGeneralSettingsWithDefault()
{
	this->CurrentLanguage = GameConstants::DEF_GENERAL_CURRENT_LANGUAGE;

	this->CurrentTextSpeed = GameConstants::DEF_GENERAL_CURRENT_TEXT_SPEED;
	UpdateTextSpeedFloatValue();
}

void AMainGameMode::InitializeGraphicalSettingsWithDefault()
{
	this->ShowFPS = GameConstants::DEF_GRAPHICAL_SHOW_FPS;
	this->IsFullScreen = GameConstants::DEF_GRAPHICAL_IS_FULL_SCREEN;
	this->GraphicalIndex = GameConstants::DEF_GRAPHICAL_GRAPHICAL_IDX;
	this->PPIndex = GameConstants::DEF_GRAPHICAL_PP_IDX;
	this->AAIndex = GameConstants::DEF_GRAPHICAL_AA_IDX;
	this->ShadowIndex = GameConstants::DEF_GRAPHICAL_SHADOW_IDX;
	this->FPSIndex = GameConstants::DEF_GRAPHICAL_FPS_IDX;
	this->ResIndex = GameConstants::DEF_GRAPHICAL_RES_IDX;
}

void AMainGameMode::InitializeAudioSettingsWithDefault()
{
	this->MasterVolumeSliderValue = GameConstants::DEF_AUDIO_MASTER_VOL_VAL;
	this->MusicVolumeSliderValue = GameConstants::DEF_AUDIO_MUSIC_VOL_VAL;
	this->EffectVolumeSliderValue = GameConstants::DEF_AUDIO_EFFECT_VOL_VAL;
}

void AMainGameMode::InitializeKeySettingsWithDefault()
{
	this->UseGamePad = GameConstants::DEF_USE_GAMEPAD;

	//GamePad
	//this->TMapGamepadKeys.Reset();
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::MoveUp, GameConstants::DEF_GPKEY_MOVE_VERTICAL);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::MoveRight, GameConstants::DEF_GPKEY_MOVE_HORIZONTAL);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::FMoveUp, GameConstants::DEF_GPKEY_FMOVE_VERTICAL);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::FMoveRight, GameConstants::DEF_GPKEY_FMOVE_HORIZONTAL);

	this->TMapGamepadKeys.Emplace(PlayerActionLabel::ACross, GameConstants::DEF_GPKEY_ACROSS);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::ATriangle, GameConstants::DEF_GPKEY_ATRIANGLE);
	this->TMapGamepadKeys.Emplace(PlayerActionLabel::AStart, GameConstants::DEF_GPKEY_ASTART);

	//Keyboard
	//this->TMapKeyboardKeys.Reset();
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::MoveUp, GameConstants::DEF_KKEY_MOVE_UP);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::MoveDown, GameConstants::DEF_KKEY_MOVE_DOWN);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::MoveLeft, GameConstants::DEF_KKEY_MOVE_LEFT);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::MoveRight, GameConstants::DEF_KKEY_MOVE_RIGHT);

	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::ACross, GameConstants::DEF_KKEY_ACROSS);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::ATriangle, GameConstants::DEF_KKEY_ATRIANGLE);
	this->TMapKeyboardKeys.Emplace(PlayerActionLabel::AStart, GameConstants::DEF_KKEY_ASTART);
}
#pragma endregion


bool AMainGameMode::GetIsBeginFunctionCompleted()
{
	return IsBeginFunctionCompleted;
}

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
	{
		CurrentTheme_AC = UGameplayStatics::SpawnSound2D(GetWorld(), MapTheme);
		CurrentTheme_AC->SetUISound(true);
		CurrentTheme_AC->FadeIn(1.0f);
	}


	//Menu
	if (IsMainMenu)
	{
		ChangeMenuWidget(StartingWidgetClass, true);
	}

	IsBeginFunctionCompleted = true;
}
//==============================================================================================
#pragma endregion

#pragma region Widgets
//==============================================================================================
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
		//OldWidgetClass = NewWidgetClass;
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
		widgetToHide->RemoveFromParent();
		widgetToHide->Destruct();
	}

	ShowCursor(showCursor);
}

void AMainGameMode::HideCurrentWidget(bool showCursor)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget->RemoveFromParent();
		CurrentWidget->Destruct();
		CurrentWidget = nullptr;
	}

	ShowCursor(showCursor);
}

UUserWidget* AMainGameMode::GetCurrentWidget()
{
	return CurrentWidget;
}

void AMainGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, bool showCursor, bool resetButtonLinks)
{
	if (CurrentWidget != nullptr)
	{
		OldWidgetClass = CurrentWidget->GetClass();
		CurrentWidget->RemoveFromViewport();
		CurrentWidget->RemoveFromParent();
		CurrentWidget->Destruct();
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

	if (resetButtonLinks)
	{
		UButtonBase::ResetButtonsInGroup();
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
//==============================================================================================
#pragma endregion

#pragma region Settings
//==============================================================================================
void AMainGameMode::UseDefaultSettings()
{
	UE_LOG(LogTemp, Log, TEXT("Use default settings"));

	InitializeGeneralSettingsWithDefault();
	InitializeGraphicalSettingsWithDefault();
	InitializeAudioSettingsWithDefault();
	InitializeKeySettingsWithDefault();

	SaveGameSettings();
	UpdateGraphicSettings();
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

bool AMainGameMode::SwitchShowMinimap()
{
	ShowMap = !ShowMap;
	return ShowMap;
}

void AMainGameMode::UpdateTextSpeedFloatValue()
{
	switch (this->CurrentTextSpeed)
	{
	case ETextSpeed::Slow:
		this->TextSpeedFloatValue = GameConstants::GENERAL_TEXT_SPEED_SLOW;
		break;
	case ETextSpeed::Fast:
		this->TextSpeedFloatValue = GameConstants::GENERAL_TEXT_SPEED_FAST;
		break;
	default:
		this->TextSpeedFloatValue = GameConstants::GENERAL_TEXT_SPEED_NORMAL;
		break;
	}
	//UE_LOG(LogTemp, Log, TEXT("TextSpeedFloatValue = %f"), this->TextSpeedFloatValue);
}

void AMainGameMode::ChangeTextSpeed(bool increase)
{
	uint8 maxIndexCurrentTextSpeed = (uint8)ETextSpeed::count;
	uint8 uint8CurrentTextSpeed = (uint8)this->CurrentTextSpeed;

	if (increase && uint8CurrentTextSpeed == maxIndexCurrentTextSpeed - 1)
		uint8CurrentTextSpeed = 0;
	else if (!increase && uint8CurrentTextSpeed == 0)
		uint8CurrentTextSpeed = maxIndexCurrentTextSpeed - 1;
	else
		increase ? ++uint8CurrentTextSpeed : --uint8CurrentTextSpeed;

	this->CurrentTextSpeed = (ETextSpeed)uint8CurrentTextSpeed;
	UpdateTextSpeedFloatValue();
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

void AMainGameMode::UpdateGraphicSettings()
{
	ExecuteConsoleCommand(*(GraphicalCommands[GraphicalIndex]));
	ExecuteConsoleCommand(*(PPCommands[PPIndex]));
	ExecuteConsoleCommand(*(AACommands[AAIndex]));
	ExecuteConsoleCommand(*(ShadowCommands[ShadowIndex]));
	ExecuteConsoleCommand(*(FPSCommands[FPSIndex]));
	ExecuteConsoleCommand(*(ResCommands[ResIndex] + (IsFullScreen ? "f" : "w")));
}

bool AMainGameMode::SwitchShowFPS()
{
	ShowFPS = !ShowFPS;
	FString showFpsCommand = "stat fps ";
	ExecuteConsoleCommand(*showFpsCommand);
	return ShowFPS;
}

bool AMainGameMode::SwitchFullScreen()
{
	IsFullScreen = !IsFullScreen;
	return IsFullScreen;
}


void AMainGameMode::SwitchPlayingAudioComponent(USoundBase* newSound)
{
	if (newSound)
	{
		CurrentTheme_AC->SetSound(newSound);
		UE_LOG(LogTemp, Log, TEXT("now playing %s"), *(newSound->GetName()));
	}
}


bool AMainGameMode::SwitchUseGamePad()
{
	UseGamePad = !UseGamePad;
	return UseGamePad;
}

void AMainGameMode::SetDeviceUsed(DeviceName deviceToUse)
{
	switch (deviceToUse)
	{
	case Keyboard:
		UseGamePad = false;
		break;
	default:
		UseGamePad = true;
		break;
	}
}

void AMainGameMode::AssignNewKey(FKey newKey, TEnumAsByte<PlayerActionLabel> actionToChange, bool isKeyboardKey)
{
	if (isKeyboardKey)
	{
		//TODO: Check if key was already used, swap with old one if it was
		this->TMapKeyboardKeys.Emplace(actionToChange, newKey);
	}

	else if (actionToChange != PlayerActionLabel::MoveDown && actionToChange != PlayerActionLabel::MoveLeft
		&& actionToChange != PlayerActionLabel::FMoveDown && actionToChange != PlayerActionLabel::FMoveLeft)
	{
		//TODO: Check if key is not already used, swap with old one if it is
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
//==============================================================================================
#pragma endregion


void AMainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

