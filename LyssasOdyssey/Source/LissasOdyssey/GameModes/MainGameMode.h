#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"


UENUM(BlueprintType)        //"BlueprintType" is essential specifier
enum GraphicLabel { Graphical, PP, AA, Shadow, FPS, Res };

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	AMainGameMode();

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;

	void InitializeSettingsMenu();

	UFUNCTION()
		void SaveCurrentMainSaveGameValues(class UMainSaveGame* SaveInstance);
	UFUNCTION()
		void LoadSettings(class UMainSaveGame * &LoadInstance);

	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget class we will use as our menu when the game end. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		TSubclassOf<UUserWidget> EndingWidgetClass;

	/** The widget class we will use as our menu when player dies. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		TSubclassOf<UUserWidget> GameOverWidgetClass;

	/** The widget class we will use as our menu when the game is paused. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		TSubclassOf<UUserWidget> PauseWidgetClass;

	/** The widget instance that we are using as our menu. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode")
		UUserWidget* CurrentWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameMode")
		APlayerController* PlayerController;

	/**widget to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidget;

	/** Assign a new key to an input if not already used */
	UFUNCTION()
		void AssignNewKey(FKey newKey, int moveToChangeIndex);

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Game_Settings")
		void SaveGameSettings();
	UFUNCTION(BlueprintCallable, Category = "Game_Settings")
		void LoadGameSettings();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu_Settings")
		FString SaveSlotName = FString(TEXT("PlayerSaveSlot"));

	UPROPERTY(EditAnywhere, Category = "Menu_Settings") bool isMenu = false;

	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	UFUNCTION(BlueprintCallable, Category = "Menu_Settings")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass, bool showCursor = false);

	UFUNCTION(BlueprintCallable, Category = "Menu_Settings")
		void ShowStartingWidget();

	UFUNCTION(BlueprintCallable, Category = "Menu_Settings")
		void ShowEndingWidget();

	UFUNCTION(BlueprintCallable, Category = "Menu_Settings")
		void ShowGameOverWidget();

	UFUNCTION(BlueprintCallable, Category = "Menu_Settings")
		void ShowPauseWidget();

	UFUNCTION(BlueprintCallable, Category = "Menu_Settings")
		void ShowHUD();

	/** Decrease if increase is false */
	UFUNCTION(BlueprintCallable, Category = "Graphic_Settings")
		void ChangeGraphicSetting(GraphicLabel graphicLabel, bool increase);

	/** Assign a new key to an input */
	UFUNCTION(BlueprintCallable, Category = "Key_Settings")
		void ListenToNewKeyForMove(int moveToChangeIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key_Settings")
		bool IsListeningToKey = false;

#pragma region game settings

#pragma region graphic settings
	////labels
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game_Settings") FText MainSettingsLabel[4];
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game_Settings") FText FPSSettingsLabel[4];
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings") FString MainSettingsLabel[4] = { "Low", "Medium","High","Ultra" };
	UPROPERTY(BlueprintReadOnly) TArray<FText> TAMainSettingsLabel;
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings") FString FPSSettingsLabel[4] = { "15Hz", "30Hz","60Hz","144Hz" };
	UPROPERTY(BlueprintReadOnly) TArray<FText> TAFPSSettingsLabel;
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings") FString ResolutionLabel[4] = { "800x600","1280x720", "1600x900", "1920x1080" };
	UPROPERTY(BlueprintReadOnly) TArray<FText> TAResolutionLabel;

	//commands
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings") FString GraphicalCommands[4] = { "r.ScreenPercentage 25", "r.ScreenPercentage 50","r.ScreenPercentage 75","r.ScreenPercentage 100" };
	TArray<FString> TAGraphicalCommands;
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings") FString PPCommands[4] = { "sg.PostProcessQuality 0","sg.PostProcessQuality 1", "sg.PostProcessQuality 2", "sg.PostProcessQuality 3" };
	TArray<FString> TAPPCommands;
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings") FString AACommands[4] = { "r.PostProcessAAQuality 0","r.PostProcessAAQuality 1", "r.PostProcessAAQuality 2", "r.PostProcessAAQuality 3" };
	TArray<FString> TAAACommands;
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings") FString ShadowCommands[4] = { "sg.ShadowQuality 0","sg.ShadowQuality 1", "sg.ShadowQuality 2", "sg.ShadowQuality 3" };
	TArray<FString> TAShadowCommands;
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings") FString FPSCommands[4] = { "t.MaxFPS 15","t.MaxFPS 30", "t.MaxFPS 60", "t.MaxFPS 144" };
	TArray<FString> TAFPSCommands;
	UPROPERTY(EditAnywhere, Category = "Graphic_Settings") FString ResCommands[4] = { "r.SetRes 800x600w","r.SetRes 1280x720w", "r.SetRes 1600x900w", "r.SetRes 1920x1080w" };
	TArray<FString> TAResCommands;
	//1280x720w ou f

	//indexes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 GraphicalIndex = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 PPIndex = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 AAIndex = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 ShadowIndex = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 FPSIndex = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		int32 ResIndex = 3;
#pragma endregion

#pragma region audio settings
	//main volume
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio_Settings")
		float MasterVolume = 1.0f;
#pragma endregion

#pragma region keybind settings

	//list of moves' label
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key_Settings")
		TArray<FString> KeyLabels = { "Move Up", "Move Down", "Move Left", "Move Right", "Start/Pause" };
	//FString KeyLabels[5] = { "Move Up", "Move Down", "Move Left", "Move Right", "Start/Pause" };

	//list of keys
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key_Settings")
		TArray<FKey> KeyList = { EKeys::W, EKeys::S, EKeys::A, EKeys::D, EKeys::Tab };
	//FKey KeyList[5] = { EKeys::W, EKeys::S, EKeys::A, EKeys::D, EKeys::Tab };

#pragma endregion

#pragma endregion


};
