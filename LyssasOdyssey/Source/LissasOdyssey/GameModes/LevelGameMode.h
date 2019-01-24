// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameModes/MainGameMode.h"
#include "Utils/Structures/StructDialogue.h"

#include "LevelGameMode.generated.h"

//enum to store the current state of gameplay
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelConstants")
		float VerticalLevel = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelConstants")
		FString LevelTitle = "LevelTitle";

#pragma region save region
	virtual void SaveGameSettings();
	virtual void LoadGameSettings();
#pragma endregion

#pragma region hud elements visibility
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		class ALyssa* Lyssa;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		class AFylgja* Fylgja;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		bool ShouldFylgjaStartVisible = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		TArray<class AFoe*> Foes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameController")
		float CollisionDistThreshold = 300.0f;

	UFUNCTION(BlueprintPure, Category = "GameController")
		ELevelPlayState GetCurrentState()const;

	UFUNCTION(BlueprintCallable, Category = "GameController")
		void SetCurrentState(ELevelPlayState newState);

#pragma region level timer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameTimer")
		float TimeLimit = 99.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameTimer")
		float LevelTimer = 0.0f;

	UFUNCTION(BlueprintPure, Category = "GameTimer")
		FText GetTimerForHud();
#pragma endregion

#pragma region Dialogue
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dialogue")
		UUserWidget* DialogueWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
		FStructDialogue CurrentDialogueToDisplay;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void ShowDialogueWidget(FStructDialogue dialogue, bool showCursor);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void HideDialogueWidget(bool showCursor = true);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void UpdateDialogue(FStructDialogue dialogue);
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		float DamageRate = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameTimer")
		float DamageRateTimer = 0.0f;

#pragma region save region
	UFUNCTION()
		virtual void SaveSettingsValues(class UMainSaveGame* SaveInstance);
	UFUNCTION()
		virtual void LoadSettingsValues(class UMainSaveGame * &LoadInstance);
#pragma endregion

private:
	void CheckForDeath();

	/**Keeps track of the current playing state*/
	UPROPERTY(VisibleAnywhere, Category = "GameController")
		ELevelPlayState Currentstate;

	/**handle any function call that rely upon changing the playing state of the game*/
	void HandleNewState(ELevelPlayState newState);

};
