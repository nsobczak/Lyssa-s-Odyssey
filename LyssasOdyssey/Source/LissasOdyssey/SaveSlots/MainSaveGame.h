// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Utils/GameEnums.h"
#include "MainSaveGame.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API UMainSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UMainSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "settings")
		FString PlayerSaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "settings")
		uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = "settings")
		FDateTime PlayerSaveSlotDate;

#pragma region Settings
	UPROPERTY(EditAnywhere, Category = "settings")
		ELanguages CurrentLanguage;

	UPROPERTY(EditAnywhere, Category = "settings")
		int32 GraphicalIndex;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 PPIndex;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 AAIndex;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 ShadowIndex;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 FPSIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		bool ShowFPS = false;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 ResolutionIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphic_Settings")
		bool IsFullScreen = false;

	UPROPERTY(EditAnywhere, Category = "settings")
		float MasterVolumeSliderValue;
	UPROPERTY(EditAnywhere, Category = "settings")
		float MusicVolumeSliderValue;
	UPROPERTY(EditAnywhere, Category = "settings")
		float EffectVolumeSliderValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Key_Settings")
		bool UseGamePad;
	UPROPERTY(EditAnywhere, Category = "settings")
		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapKeyboardKeys;
	UPROPERTY(EditAnywhere, Category = "settings")
		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapGamepadKeys;
#pragma endregion

#pragma region Game

	UPROPERTY(EditAnywhere, Category = "game")
		TMap<TEnumAsByte<LevelLabels>, int>TMapPlayerPickupByLevel;

	UPROPERTY(VisibleAnywhere, Category = "game")
		bool IsInitialized_TMapPlayerPickupByLevel;
#pragma endregion
};
