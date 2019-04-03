#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SaveSlots/MainSaveGame.h"
#include "MainGameMode.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	AMainGameMode();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "GameControl")
		virtual bool GetIsBeginFunctionCompleted();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode")
		APlayerController* PlayerController;

	UFUNCTION(BlueprintCallable, Category = "utils")
		bool ExecuteConsoleCommand(FString consoleCommand);//static

#pragma region save region
	UFUNCTION(BlueprintCallable, Category = "Game_Settings")
		virtual bool SaveGameSettings();
	UFUNCTION(BlueprintCallable, Category = "Game_Settings")
		virtual bool LoadGameSettings();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu_Settings")
		bool DoesSaveGameExist = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		FString SaveSlotName = FString(TEXT("PlayerSaveSlot"));
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		bool IsPauseAllowed = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		bool IsMainMenu = false;

#pragma region widget functions
	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void ShowCursor(bool showCursor);

	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void ShowWidget(TSubclassOf<UUserWidget> NewWidgetClass, bool showCursor);

	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void HideWidget(UUserWidget* widgetToHide, bool showCursor);

	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void HideCurrentWidget(bool showCursor);

	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, bool showCursor = false);

	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void ShowStartingWidget();

	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void ShowEndingWidget();

	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void ShowGameOverWidget();

	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void ShowPauseWidget();

	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void ShowHUD();

#pragma endregion

#pragma region game settings

#pragma region hud elements visibility
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelHud")
		bool ShowMap = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelHud")
		bool ShowTime = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelHud")
		bool ShowLife = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelHud")
		bool ShowTitle = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelHud")
		bool ShowPickup = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelHud")
		bool ShowMouseCursorInLevel = true;
#pragma endregion


	UFUNCTION(BlueprintCallable, Category = "Game_Settings")
		void UseDefaultSettings();

#pragma region general settings
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "General_Settings")
		ELanguages CurrentLanguage;
	/** Decrease if increase is false */
	UFUNCTION(BlueprintCallable, Category = "General_Settings")
		void ChangeCurrentLanguage(bool increase);

	UFUNCTION(BlueprintCallable, Category = "General_Settings")
		bool SwitchShowMinimap();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "General_Settings")
		ETextSpeed CurrentTextSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General_Settings")
		float TextSpeedFloatValue;
	UFUNCTION()
		void UpdateTextSpeedFloatValue();
	UFUNCTION(BlueprintCallable, Category = "General_Settings")
		void ChangeTextSpeed(bool increase);
#pragma endregion

#pragma region graphic settings
	/** Decrease if increase is false, increase otherwise */
	UFUNCTION(BlueprintCallable, Category = "Graphic_Settings")
		void ChangeGraphicSetting(GraphicLabel graphicLabel, bool increase);

	/** Execute all actual graphic settings*/
	UFUNCTION(BlueprintCallable, Category = "Graphic_Settings")
		void UpdateGraphicSettings();

	//commands
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString GraphicalCommands[4] = { "r.ScreenPercentage 25", "r.ScreenPercentage 50","r.ScreenPercentage 75","r.ScreenPercentage 100" };
	TArray<FString> TAGraphicalCommands;

	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString PPCommands[4] = { "sg.PostProcessQuality 0", "sg.PostProcessQuality 1", "sg.PostProcessQuality 2", "sg.PostProcessQuality 3" };
	TArray<FString> TAPPCommands;

	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString AACommands[4] = { "r.PostProcessAAQuality 0", "r.PostProcessAAQuality 1", "r.PostProcessAAQuality 2", "r.PostProcessAAQuality 3" };
	TArray<FString> TAAACommands;

	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString ShadowCommands[4] = { "sg.ShadowQuality 0", "sg.ShadowQuality 1", "sg.ShadowQuality 2", "sg.ShadowQuality 3" };
	TArray<FString> TAShadowCommands;

	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString FPSCommands[4] = { "t.MaxFPS 15", "t.MaxFPS 30", "t.MaxFPS 60", "t.MaxFPS 144" };
	TArray<FString> TAFPSCommands;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		bool ShowFPS = false;
	UFUNCTION(BlueprintCallable, Category = "Graphic_Settings")
		bool SwitchShowFPS();

	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString ResCommands[4] = { "r.SetRes 800x600","r.SetRes 1280x720", "r.SetRes 1600x900", "r.SetRes 1920x1080" };
	TArray<FString> TAResCommands;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		bool IsFullScreen;
	UFUNCTION(BlueprintCallable, Category = "Graphic_Settings")
		bool SwitchFullScreen();

	//indexes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 GraphicalIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 PPIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 AAIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 ShadowIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 FPSIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 ResIndex;
#pragma endregion

#pragma region audio settings
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio_Settings", AdvancedDisplay)
		float MasterVolumeSliderValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio_Settings", AdvancedDisplay)
		float MusicVolumeSliderValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Audio_Settings", AdvancedDisplay)
		float EffectVolumeSliderValue;

	UFUNCTION(BlueprintCallable, Category = "Audio_Settings")
		void UpdateAudioVolumes();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
		class USoundMix* SoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
		class USoundClass *  SCMusic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
		class USoundClass *  SCEffect;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
	//	UAudioComponent* GameModeMusic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
		USoundBase * MapTheme;
#pragma endregion

#pragma region keybind settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key_Settings")
		bool UseGamePad;
	UFUNCTION(BlueprintCallable, Category = "Key_Settings")
		bool SwitchUseGamePad();
	UFUNCTION(BlueprintCallable, Category = "Key_Settings")
		void SetDeviceUsed(DeviceName deviceToUse);

	/** Assign a new key to an input */
	UFUNCTION(BlueprintCallable, Category = "Key_Settings")
		void ListenToNewKeyForMove(TEnumAsByte<PlayerActionLabel> actionToChange, int iteration);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Key_Settings")
		bool IsListeningToKey = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Key_Settings")
		TEnumAsByte<PlayerActionLabel> ListeningToKeyLabel;

	/**list of keys*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key_Settings")
		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapKeyboardKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key_Settings")
		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapGamepadKeys;
#pragma endregion

#pragma endregion

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;

	void InitializeGeneralSettingsWithDefault();
	void InitializeGraphicalSettingsWithDefault();
	void InitializeAudioSettingsWithDefault();
	void InitializeKeySettingsWithDefault();

	void InitializeTArrayAndApplyGraphicalSettings();

	virtual void SaveSettingsValues(class UMainSaveGame* SaveInstance);
	virtual void LoadSettingsValues(class UMainSaveGame * &LoadInstance);

	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Widget")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget class we will use as our menu when the game end. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Widget")
		TSubclassOf<UUserWidget> EndingWidgetClass;

	/** The widget class we will use as our menu when player dies. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Widget")
		TSubclassOf<UUserWidget> GameOverWidgetClass;

	/** The widget class we will use as our menu when the game is paused. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Widget")
		TSubclassOf<UUserWidget> PauseWidgetClass;

	/** The widget instance that we are using as our menu. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu_Widget")
		UUserWidget* CurrentWidget;

	/** The widget instance that we used. (useful for return buttons) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu_Widget")
		UUserWidget* OldWidget;

	/**widget to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Menu_Widget", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidget;

	/** Assign a new key to an input if not already used */
	void AssignNewKey(FKey newKey, TEnumAsByte<PlayerActionLabel> actionToChange, bool isKeyboardKey = true);

private:
	bool IsBeginFunctionCompleted = false;
};
