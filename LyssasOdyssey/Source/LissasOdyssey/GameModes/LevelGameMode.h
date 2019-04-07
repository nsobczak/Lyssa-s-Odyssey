#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameModes/MainGameMode.h"
#include "Utils/Structures/StructDialogue.h"
#include "Utils/GameEnums.h"

#include "LevelGameMode.generated.h"

/**enum to store the current state of gameplay*/
UENUM(BlueprintType)
enum class ELevelPlayState : uint8
{
	EPlaying,
	EPause,
	EGameOver,
	ELevelCompleted,
	EUnknown
};

UCLASS()
class LISSASODYSSEY_API ALevelGameMode : public AMainGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual bool GetIsBeginFunctionCompleted() override;

	/** Vertical offset for shots*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelConstants")
		float VerticalLevel = 50.0f;


#pragma region exec functions
	UFUNCTION(exec)
		int GetScorePickupCountInLevel();
	UFUNCTION(exec)
		int GetScorePickupValueInLevel();
	UFUNCTION(exec, BlueprintPure, Category = "GameControl")
		int GetStaticTotalScoreInLevel();
	UFUNCTION(exec)
		void AddScorePickup(int amount);

	UFUNCTION(BlueprintCallable, Category = "GameControl")
		bool AddKey(TEnumAsByte<KeyLockNature> keyNature);
	UFUNCTION(exec)
		bool AddBlueKey();
	UFUNCTION(exec)
		bool AddRedKey();
	UFUNCTION(exec)
		bool AddYellowKey();
	UFUNCTION(exec)
		bool AddGreenKey();
	UFUNCTION(exec)
		bool AddPurpleKey();
#pragma endregion

	/** Level nature*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelConstants")
		TEnumAsByte<LevelLabels> LevelLabel;

	/** Boss theme for the map*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio_Settings")
		USoundBase* BossTheme;//replace by tArray if more than 1 boss in same level	

#pragma region save region
	virtual bool SaveGameSettings() override;
	virtual bool LoadGameSettings() override;
#pragma endregion


#pragma region Map
	/** Map texture*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
		UTexture2D* Map;

	/** Player icon on map*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
		UTexture2D* PlayerIcon;
#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController", AdvancedDisplay)
		class ALyssa* Lyssa;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController", AdvancedDisplay)
		class AFylgja* Fylgja;
	/** Hide fylgja if false, useful for tutorial*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		bool ShouldFylgjaStartVisible = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameController", AdvancedDisplay)
		class UHUD_BossInfo* HudBossInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameController", AdvancedDisplay)
		class ABoss* CurrentBoss;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameController", AdvancedDisplay)
		bool IsBossActive = false;

	UFUNCTION(BlueprintPure, Category = "GameController")
		ELevelPlayState GetCurrentState()const;

	UFUNCTION(BlueprintCallable, Category = "GameController")
		void SetCurrentState(ELevelPlayState newState);

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD_boss")
	//	bool WasGamePaused = false;

	/** Level completed callback*/
	UFUNCTION(BlueprintImplementableEvent, Category = "GameController")
		void OnLevelCompleted();

	/** Game over callback*/
	UFUNCTION(BlueprintImplementableEvent, Category = "GameController")
		void OnGameOver();


#pragma region level timer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameTimer")
		float LevelTimer = 0.0f;

	UFUNCTION(BlueprintPure, Category = "GameTimer")
		FText GetTimerForHud();
#pragma endregion


#pragma region Dialogue
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dialogue")
		UUserWidget* DialogueWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
		FString CurrentDialogueText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
		FStructDialogue CurrentDialogueToDisplay;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void ShowDialogueWidget(bool showCursor = true);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void HideDialogueWidget(bool showCursor = true);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void UpdateCurrentDialogueText(FString newDialogueText);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void UpdateDialoguePictureStruct(FStructTextureData dialogue);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void UpdateDialogueSpeakerName(DialogueCharacterName dialogueText);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void UpdateDialogueTextStruct(FStructTextTrad dialogueText);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void UpdateDialogue(FStructDialogue dialogue);
#pragma endregion


protected:
#pragma region save region
	virtual void SaveSettingsValues(class UMainSaveGame* SaveInstance) override;
	virtual void LoadSettingsValues(class UMainSaveGame * &LoadInstance) override;
#pragma endregion

private:
	void CheckForDeath();

	/**Keeps track of the current playing state*/
	UPROPERTY(VisibleAnywhere, Category = "GameController")
		ELevelPlayState CurrentState;

	/**handle any function call that rely upon changing the playing state of the game*/
	void HandleNewState(ELevelPlayState newState);

	bool IsBeginFunctionCompleted = false;
};
