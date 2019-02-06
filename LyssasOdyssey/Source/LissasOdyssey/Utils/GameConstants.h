// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/GameEnums.h"
#include "InputCore/Classes/InputCoreTypes.h"

//#include "GameConstants.generated.h"

/**
 *
 */
class LISSASODYSSEY_API GameConstants
{
public:
	GameConstants();
	~GameConstants();

	//TODO: use caps lock
	static const FColor COLOR_TEXT_LIGHT_GRAY;

#pragma region Settings
#pragma region Defaults general settings
	static const ELanguages DEF_GENERAL_CURRENT_LANGUAGE;
#pragma endregion

#pragma region Defaults graphics settings
	static const bool DEF_GRAPHICAL_SHOW_FPS;
	static const bool DEF_GRAPHICAL_IS_FULL_SCREEN;
	static const int32 DEF_GRAPHICAL_GRAPHICAL_IDX;
	static const int32 DEF_GRAPHICAL_PP_IDX;
	static const int32 DEF_GRAPHICAL_AA_IDX;
	static const int32 DEF_GRAPHICAL_SHADOW_IDX;
	static const int32 DEF_GRAPHICAL_FPS_IDX;
	static const int32 DEF_GRAPHICAL_RES_IDX;
#pragma endregion

#pragma region Defaults audio settings
	static const float DEF_AUDIO_MASTER_VOL_VAL;
	static const float DEF_AUDIO_MUSIC_VOL_VAL;
	static const float DEF_AUDIO_EFFECT_VOL_VAL;
#pragma endregion

#pragma region Defaults keybind settings
	static const bool DEF_USE_GAMEPAD;

	//GamePad
	static const FKey DEF_GPKEY_MOVE_VERTICAL;
	static const FKey DEF_GPKEY_MOVE_HORIZONTAL;
	static const FKey DEF_GPKEY_FMOVE_VERTICAL;
	static const FKey DEF_GPKEY_FMOVE_HORIZONTAL;

	static const FKey DEF_GPKEY_ACROSS;
	static const FKey DEF_GPKEY_ATRIANGLE;
	static const FKey DEF_GPKEY_ASTART;

	//Keyboard
	static const FKey DEF_KKEY_MOVE_UP;
	static const FKey DEF_KKEY_MOVE_RIGHT;
	static const FKey DEF_KKEY_MOVE_DOWN;
	static const FKey DEF_KKEY_MOVE_LEFT;

	static const FKey DEF_KKEY_ACROSS;
	static const FKey DEF_KKEY_ATRIANGLE;
	static const FKey DEF_KKEY_ASTART;
#pragma endregion  
#pragma endregion


#pragma region Game
	//level titles
	static const FString LVL_TITLE_DEFAULT;
	static const FString LVL_TITLE_CANYON;
	static const FString LVL_TITLE_HUB;
	static const FString LVL_TITLE_FOREST;
	static const FString LVL_TITLE_ICE;
	static const FString LVL_TITLE_VOLCANO;

	//Game
	static const int PICKUP_SCORE_MAX_CANYON;
	static const int PICKUP_SCORE_MAX_FOREST;
	static const int PICKUP_SCORE_MAX_ICE;
	static const int PICKUP_SCORE_MAX_VOLCANO;
#pragma endregion

};
