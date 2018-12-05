// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameModes/MainGameMode.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Characters/Foes/Foe.h"
#include "WorldAssets/FinishArea.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameSettings")
		bool ShowMouseCursorInLevel = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		AFinishArea* FinishArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		ALyssa* Lyssa;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		TArray<AFoe*> Foes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameController")
		float CollisionDistThreshold = 300.0f;

	UFUNCTION(BlueprintCallable, Category = "GameController")
		void CheckForLevelCompleted();

	UFUNCTION(BlueprintPure, Category = "GameController")
		ELevelPlayState GetCurrentState()const;

	UFUNCTION(BlueprintCallable, Category = "GameController")
		void SetCurrentState(ELevelPlayState newState);

	//TODO: add infos here
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameTimer")
		float TimeLimit = 99.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameTimer")
		float LevelTimer = 0.0f;

	UFUNCTION(BlueprintPure, Category = "GameTimer")
		FText GetTimerForHud();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
		UUserWidget* DialogueWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
		FStructDialogue CurrentDialogueToDisplay;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void ShowDialogueWidget(FStructDialogue dialogue, bool showCursor);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void HideDialogueWidget(bool showCursor = true);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void UpdateDialogue(FStructDialogue dialogue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		float DamageRate = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameTimer")
		float DamageRateTimer = 0.0f;

private:
	void CheckForDeath();

	/**Keeps track of the current playing state*/
	UPROPERTY(VisibleAnywhere, Category = "GameController")
		ELevelPlayState Currentstate;

	/**handle any function call that rely upon changing the playing state of the game*/
	void HandleNewState(ELevelPlayState newState);

};
