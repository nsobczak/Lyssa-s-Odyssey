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

	/** call this to know if begin function has been completed*/
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
	UFUNCTION(BlueprintImplementableEvent, Category = "Menu_Settings")
		void ShouldShowDeviceSelectionWidget();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		FString SaveSlotName = FString(TEXT("PlayerSaveSlot"));
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		bool IsPauseAllowed = true;

	/** show StartingWidgetClass at launch if true*/
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

	/** The widget instance that we are using as our menu. */
	UFUNCTION(BlueprintPure, Category = "Widget_Functions")
		UUserWidget* GetCurrentWidget();

	/** Remove the current menu widget (if not null) and create a new one from the specified class, if provided.
	Show cursor if true. Reset button links if true.*/
	UFUNCTION(BlueprintCallable, Category = "Widget_Functions")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, bool showCursor = false, bool resetButtonLinks = true);

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

	/** Save current settings and restart level to apply them*/
	UFUNCTION(BlueprintCallable, Category = "Game_Settings")
		void ApplySettings();

	/** Reset all settings to default values and apply them*/
	UFUNCTION(BlueprintCallable, Category = "Game_Settings")
		void UseDefaultSettings();


#pragma region general settings
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "General_Settings")
		ELanguages CurrentLanguage;
	/** Decrease if increase is false */
	UFUNCTION(BlueprintCallable, Category = "General_Settings")
		void ChangeCurrentLanguage(bool increase);

	/** Switch ShowMinimap value*/
	UFUNCTION(BlueprintCallable, Category = "General_Settings")
		bool SwitchShowMinimap();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "General_Settings")
		ETextSpeed CurrentTextSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General_Settings")
		float TextSpeedFloatValue;
	UFUNCTION(BlueprintCallable, Category = "General_Settings")
		void UpdateTextSpeedFloatValue();
	UFUNCTION(BlueprintCallable, Category = "General_Settings")
		void ChangeTextSpeed(bool increase);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "General_Settings")
		int LyssaBodyMatIdx;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "General_Settings")
		int LyssaShapeMatIdx;
#pragma endregion

#pragma region graphic settings
	/** Decrease if increase is false, increase otherwise */
	UFUNCTION(BlueprintCallable, Category = "Graphic_Settings")
		void ChangeGraphicSetting(GraphicLabel graphicLabel, bool increase);

	/** Execute all actual graphic settings commands*/
	UFUNCTION(BlueprintCallable, Category = "Graphic_Settings")
		void UpdateGraphicSettings();

	//commands
	/** ScreenPercentage */
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString GraphicalCommands[4] = { "r.ScreenPercentage 25", "r.ScreenPercentage 50","r.ScreenPercentage 75","r.ScreenPercentage 100" };
	TArray<FString> TAGraphicalCommands;
	/** ScreenPercentage command index in list*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 GraphicalIndex;

	/** PostProcessQuality */
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString PPCommands[4] = { "sg.PostProcessQuality 0", "sg.PostProcessQuality 1", "sg.PostProcessQuality 2", "sg.PostProcessQuality 3" };
	TArray<FString> TAPPCommands;
	/** PostProcessQuality command index in list*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 PPIndex;

	/** PostProcessAAQuality */
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString AACommands[4] = { "r.PostProcessAAQuality 0", "r.PostProcessAAQuality 1", "r.PostProcessAAQuality 2", "r.PostProcessAAQuality 3" };
	TArray<FString> TAAACommands;
	/** PostProcessAAQuality command index in list*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 AAIndex;

	/** ShadowQuality */
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString ShadowCommands[4] = { "sg.ShadowQuality 0", "sg.ShadowQuality 1", "sg.ShadowQuality 2", "sg.ShadowQuality 3" };
	TArray<FString> TAShadowCommands;
	/** ShadowQuality command index in list*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 ShadowIndex;

	/** MaxFPS */
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString FPSCommands[4] = { "t.MaxFPS 15", "t.MaxFPS 30", "t.MaxFPS 60", "t.MaxFPS 144" };
	TArray<FString> TAFPSCommands;
	/** MaxFPS command index in list*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 FPSIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		bool ShowFPS = false;
	/** Switch ShowFPS value*/
	UFUNCTION(BlueprintCallable, Category = "Graphic_Settings")
		bool SwitchShowFPS();

	/** Resolution */
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings", AdvancedDisplay)
		FString ResCommands[4] = { "r.SetRes 800x600","r.SetRes 1280x720", "r.SetRes 1600x900", "r.SetRes 1920x1080" };
	TArray<FString> TAResCommands;
	/** Resolution command index in list*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 ResIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		bool IsFullScreen;
	/** Switch IsFullScreen value*/
	UFUNCTION(BlueprintCallable, Category = "Graphic_Settings")
		bool SwitchFullScreen();

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

	/** USoundClass for themes*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
		class USoundClass*  SCMusic;
	/** USoundClass for sound effects*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
		class USoundClass*  SCEffect;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
	//	UAudioComponent* GameModeMusic;

	/** Main theme for the map*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
		USoundBase* MapTheme;

	/** Other theme for when we want to use another one*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings", AdvancedDisplay)
		USoundBase* OtherTheme;//replace by tArray if more than 1 boss in same level	

	/** AudioComponent currently playing*/
	UPROPERTY(BlueprintReadOnly, Category = "Audio_Settings")
		UAudioComponent* CurrentTheme_AC;

	/** Change CurrentTheme_AC for new one if provided*/
	UFUNCTION(BlueprintCallable, Category = "Audio_Settings")
		void SwitchPlayingAudioComponent(USoundBase* newSound);

#pragma endregion

#pragma region keybind settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key_Settings")
		bool UseGamePad;
	/** Switch UseGamePad value*/
	UFUNCTION(BlueprintCallable, Category = "Key_Settings")
		bool SwitchUseGamePad();
	UFUNCTION(BlueprintCallable, Category = "Key_Settings")
		void SetDeviceUsed(DeviceName deviceToUse);

	/** Assign a new key to an input */
	UFUNCTION(BlueprintCallable, Category = "Key_Settings")
		void ListenToNewKeyForMove(TEnumAsByte<PlayerActionLabel> actionToChange, int iteration);

	/** Value to know if we are currently listening to a new key to assign or not*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Key_Settings")
		bool IsListeningToKey = false;
	/** Current action to change*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Key_Settings")
		TEnumAsByte<PlayerActionLabel> ListeningToKeyLabel;

	/**list of Keyboard keys*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key_Settings")
		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapKeyboardKeys;
	/**list of Gamepad keys*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key_Settings")
		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapGamepadKeys;
#pragma endregion

#pragma endregion

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;

	UFUNCTION()
		void InitializeGeneralSettingsWithDefault();
	UFUNCTION()
		void InitializeGraphicalSettingsWithDefault();
	UFUNCTION()
		void InitializeAudioSettingsWithDefault();
	UFUNCTION()
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
		TSubclassOf<UUserWidget> OldWidgetClass;

	/**widget to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Menu_Widget", Meta = (BlueprintProtected = "true"))
		TSubclassOf<UUserWidget> HUDWidget;

	/** Assign a new key to an input if not already used */
	void AssignNewKey(FKey newKey, TEnumAsByte<PlayerActionLabel> actionToChange, bool isKeyboardKey = true);

private:
	bool IsBeginFunctionCompleted = false;
};
