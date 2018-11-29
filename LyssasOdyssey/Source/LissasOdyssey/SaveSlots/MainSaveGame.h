// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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
		int32 GraphicalIndex;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 PPIndex;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 AAIndex;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 ShadowIndex;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 FPSIndex;
	UPROPERTY(EditAnywhere, Category = "settings")
		int32 ResolutionIndex;

	UPROPERTY(EditAnywhere, Category = "settings")
		float MasterVolumeSliderValue;

	UPROPERTY(EditAnywhere, Category = "settings")
		TArray<FKey> PlayerKeys;
#pragma endregion
};
