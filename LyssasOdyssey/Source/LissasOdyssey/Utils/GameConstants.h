// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/GameEnums.h"
//#include "GameConstants.generated.h"

/**
 *
 */
class LISSASODYSSEY_API GameConstants
{
public:
	GameConstants();
	~GameConstants();

	static const FColor ColorTextLightGray;

#pragma region Settings
#pragma region Defaults general settings
	static const ELanguages DefaultGeneral_CurrentLanguage;
#pragma endregion

#pragma region Defaults graphics settings
	static const bool DefaultGraphical_ShowFPS;
	static const bool DefaultGraphical_IsFullScreen;
	static const int32 DefaultGraphical_GraphicalIndex;
	static const int32 DefaultGraphical_PPIndex;
	static const int32 DefaultGraphical_AAIndex;
	static const int32 DefaultGraphical_ShadowIndex;
	static const int32 DefaultGraphical_FPSIndex;
	static const int32 DefaultGraphical_ResIndex;
#pragma endregion

#pragma region Defaults audio settings
	static const float DefaultAudio_MasterVolumeSliderValue;
	static const float DefaultAudio_MusicVolumeSliderValue;
	static const float DefaultAudio_EffectVolumeSliderValue;
#pragma endregion

#pragma region Defaults keybind settings
	static const bool UseGamePad;

	//GamePad
	static const FKey DefaultGPKey_MoveVertical;
	static const FKey DefaultGPKey_MoveHorizontal;
	static const FKey DefaultGPKey_FMoveVertical;
	static const FKey DefaultGPKey_FMoveHorizontal;

	static const FKey DefaultGPKey_ACross;
	static const FKey DefaultGPKey_ATriangle;
	static const FKey DefaultGPKey_AStart;

	//Keyboard
	static const FKey DefaultKKey_MoveUp;
	static const FKey DefaultKKey_MoveRight;
	static const FKey DefaultKKey_MoveDown;
	static const FKey DefaultKKey_MoveLeft;

	static const FKey DefaultKKey_ACross;
	static const FKey DefaultKKey_ATriangle;
	static const FKey DefaultKKey_AStart;
#pragma endregion  
#pragma endregion


#pragma region Game
	//level titles
	static const FString LevelTitle_Default;
	static const FString LevelTitle_Canyon;
	static const FString LevelTitle_Hub;
	static const FString LevelTitle_Forest;
	static const FString LevelTitle_Ice;
	static const FString LevelTitle_Volcano;

	//Game
	static const int CanyonPickupAmount;
	static const int ForestPickupAmount;
	static const int IcePickupAmount;
	static const int VolcanoPickupAmount;
#pragma endregion

};
