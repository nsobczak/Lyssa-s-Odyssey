#include "MainGameMode.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Blueprint/UserWidget.h"

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
	SaveInstance->ResolutionIndex = this->ResIndex;

	SaveInstance->MasterVolumeSliderValue = this->MasterVolumeSliderValue;
	SaveInstance->MusicVolumeSliderValue = this->MusicVolumeSliderValue;
	SaveInstance->EffectVolumeSliderValue = this->EffectVolumeSliderValue;

	SaveInstance->PlayerKeys = this->KeyList;

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
	LoadInstance = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	this->CurrentLanguage = LoadInstance->CurrentLanguage;

	this->GraphicalIndex = LoadInstance->GraphicalIndex;
	this->PPIndex = LoadInstance->PPIndex;
	this->AAIndex = LoadInstance->AAIndex;
	this->ShadowIndex = LoadInstance->ShadowIndex;
	this->FPSIndex = LoadInstance->FPSIndex;
	this->ResIndex = LoadInstance->ResolutionIndex;

	this->MasterVolumeSliderValue = LoadInstance->MasterVolumeSliderValue;
	this->MusicVolumeSliderValue = LoadInstance->MusicVolumeSliderValue;
	this->EffectVolumeSliderValue = LoadInstance->EffectVolumeSliderValue;

	if (this->KeyList.Num() == LoadInstance->PlayerKeys.Num())
		this->KeyList = LoadInstance->PlayerKeys;

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
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString(TEXT("No save game found.")), true);
}
#pragma endregion


void AMainGameMode::UpdateAudioVolumes()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SCMusic, MusicVolumeSliderValue, 1.0f, 0, true);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SCEffect, EffectVolumeSliderValue, 1.0f, 0, true);
}


void AMainGameMode::InitializeSettingsMenu()
{
	//command
	TAGraphicalCommands.Append(GraphicalCommands, ARRAY_COUNT(GraphicalCommands));
	TAPPCommands.Append(GraphicalCommands, ARRAY_COUNT(PPCommands));
	TAAACommands.Append(GraphicalCommands, ARRAY_COUNT(AACommands));
	TAShadowCommands.Append(GraphicalCommands, ARRAY_COUNT(ShadowCommands));
	TAFPSCommands.Append(GraphicalCommands, ARRAY_COUNT(FPSCommands));
	TAResCommands.Append(ResCommands, ARRAY_COUNT(ResCommands));

	//TMP
	FString Final0 = "r.ScreenPercentage 50";
	GetWorld()->Exec(GetWorld(), *Final0);
	FString Final1 = "sg.PostProcessQuality 1";
	GetWorld()->Exec(GetWorld(), *Final1);
	FString Final2 = "r.PostProcessAAQuality 1";
	GetWorld()->Exec(GetWorld(), *Final2);
	FString Final3 = "sg.ShadowQuality 1";
	GetWorld()->Exec(GetWorld(), *Final3);
	FString Final4 = "t.MaxFPS 30";
	GetWorld()->Exec(GetWorld(), *Final4);
}


void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->bAutoManageActiveCameraTarget = false;

	//load saved settings here
	LoadGameSettings();

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

	//menu
	if (IsMenu)
	{
		InitializeSettingsMenu();
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
	int32 commandIndex = 0;

	//index
	switch (graphicLabel)
	{
	case Graphical:
		increase ? ++GraphicalIndex : --GraphicalIndex;
		commandList = TAGraphicalCommands;
		GraphicalIndex = (GraphicalIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(GraphicalIndex < 0) ? 0 : GraphicalIndex;
		commandIndex = GraphicalIndex;
		break;

	case PP:
		increase ? ++PPIndex : --PPIndex;
		commandList = TAPPCommands;
		PPIndex = (PPIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(PPIndex < 0) ? 0 : PPIndex;
		commandIndex = PPIndex;
		break;

	case AA:
		increase ? ++AAIndex : --AAIndex;
		commandList = TAAACommands;
		AAIndex = (AAIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(AAIndex < 0) ? 0 : AAIndex;
		commandIndex = AAIndex;
		break;

	case Shadow:
		increase ? ++ShadowIndex : --ShadowIndex;
		commandList = TAShadowCommands;
		ShadowIndex = (ShadowIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(ShadowIndex < 0) ? 0 : ShadowIndex;
		commandIndex = ShadowIndex;
		break;

	case FPS:
		increase ? ++FPSIndex : --FPSIndex;
		commandList = TAFPSCommands;
		FPSIndex = (FPSIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(FPSIndex < 0) ? 0 : FPSIndex;
		commandIndex = FPSIndex;
		break;

	case Res:
		increase ? ++ResIndex : --ResIndex;
		commandList = TAResCommands;
		ResIndex = (ResIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(ResIndex < 0) ? 0 : ResIndex;
		commandIndex = ResIndex;
		break;

	default:
		break;
	}

	//command
	if (commandIndex >= 0 && commandIndex < commandList.Num()) //check for safety
		GetWorld()->Exec(GetWorld(), *(commandList[commandIndex]));
}

void AMainGameMode::AssignNewKey(FKey newKey, int moveToChangeIndex)
{
	//Check if key is not already used, swap with old one if it is
	if (this->KeyList.Contains(newKey))
	{
		this->KeyList.Swap(moveToChangeIndex, this->KeyList.IndexOfByKey(newKey));
	}
	else
	{
		this->KeyList.RemoveAt(moveToChangeIndex);
		this->KeyList.Insert(newKey, moveToChangeIndex);
	}
}

void AMainGameMode::ListenToNewKeyForMove(int moveToChangeIndex, int iteration)
{
	if (moveToChangeIndex >= this->KeyList.Num() || iteration >= 50) { IsListeningToKey = false;  return; }

	FKey boundKey = this->KeyList[moveToChangeIndex];
	IsListeningToKey = true;
	ListeningToKeyIndex = moveToChangeIndex;

	if (PlayerController->IsInputKeyDown(EKeys::AnyKey))//if any key pressed
	{
		//retrieve any key pressed
		FKey keyPressed;
		TArray<FKey> allKeys;
		EKeys::GetAllKeys(allKeys);
		for (size_t i = 0; i < allKeys.Num(); ++i)
		{
			if (allKeys[i] != EKeys::AnyKey && PlayerController->IsInputKeyDown(allKeys[i]))
			{
				keyPressed = allKeys[i];
				FString keyPressedString = keyPressed.ToString();
				UE_LOG(LogTemp, Warning, TEXT("key %s was pressed"), *keyPressedString);
				if (keyPressed.IsGamepadKey())
					UE_LOG(LogTemp, Log, TEXT("key %s is gamepadKey"), *keyPressedString);

				AssignNewKey(keyPressed, moveToChangeIndex);
				boundKey = keyPressed;
				IsListeningToKey = false;

				return;
			}
		}

		//TODO: block ui input when IsListeningToKey is true to prevent user from changing menu
	}

	//delay
	FTimerHandle TimerHandle; // Handle to manage the timer
	FTimerDelegate TimerDel; //Bind function with parameters
	TimerDel.BindUFunction(this, FName("ListenToNewKeyForMove"), moveToChangeIndex, iteration + 1);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.1f, false);
}

void AMainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
