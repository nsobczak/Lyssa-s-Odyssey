// Fill out your copyright notice in the Description page of Project Settings.

#include "GameConstants.h"

const FColor GameConstants::COLOR_TEXT_LIGHT_GRAY = FColor::FromHex("#606060FF");

#pragma region Game Settings

#pragma region Defaults general settings
const ELanguages GameConstants::DEF_GENERAL_CURRENT_LANGUAGE = ELanguages::en;
#pragma endregion

#pragma region Defaults graphics settings
const bool GameConstants::DEF_GRAPHICAL_SHOW_FPS = false;
const bool GameConstants::DEF_GRAPHICAL_IS_FULL_SCREEN = false;
const int32 GameConstants::DEF_GRAPHICAL_GRAPHICAL_IDX = 3;
const int32 GameConstants::DEF_GRAPHICAL_PP_IDX = 3;
const int32 GameConstants::DEF_GRAPHICAL_AA_IDX = 3;
const int32 GameConstants::DEF_GRAPHICAL_SHADOW_IDX = 3;
const int32 GameConstants::DEF_GRAPHICAL_FPS_IDX = 2;
const int32 GameConstants::DEF_GRAPHICAL_RES_IDX = 3;
#pragma endregion

#pragma region Defaults audio settings
const float GameConstants::DEF_AUDIO_MASTER_VOL_VAL = 1.0f;
const float GameConstants::DEF_AUDIO_MUSIC_VOL_VAL = 1.0f;
const float GameConstants::DEF_AUDIO_EFFECT_VOL_VAL = 1.0f;
#pragma endregion

#pragma region Defaults keybind settings
const bool GameConstants::DEF_USE_GAMEPAD = true;

//GamePad
const FKey GameConstants::DEF_GPKEY_MOVE_VERTICAL = EKeys::Gamepad_LeftY;
const FKey GameConstants::DEF_GPKEY_MOVE_HORIZONTAL = EKeys::Gamepad_LeftX;
const FKey GameConstants::DEF_GPKEY_FMOVE_VERTICAL = EKeys::Gamepad_RightY;
const FKey GameConstants::DEF_GPKEY_FMOVE_HORIZONTAL = EKeys::Gamepad_RightX;

const FKey GameConstants::DEF_GPKEY_ACROSS = EKeys::Gamepad_FaceButton_Bottom;
const FKey GameConstants::DEF_GPKEY_ATRIANGLE = EKeys::Gamepad_FaceButton_Right;
const FKey GameConstants::DEF_GPKEY_ASTART = EKeys::Gamepad_Special_Right;

//Keyboard
const FKey GameConstants::DEF_KKEY_MOVE_UP = EKeys::W;
const FKey GameConstants::DEF_KKEY_MOVE_RIGHT = EKeys::D;
const FKey GameConstants::DEF_KKEY_MOVE_DOWN = EKeys::S;
const FKey GameConstants::DEF_KKEY_MOVE_LEFT = EKeys::A;

const FKey GameConstants::DEF_KKEY_ACROSS = EKeys::E;
const FKey GameConstants::DEF_KKEY_ATRIANGLE = EKeys::R;
const FKey GameConstants::DEF_KKEY_ASTART = EKeys::T;
#pragma endregion

#pragma endregion

#pragma region Game
//level titles
const FString  GameConstants::LVL_TITLE_DEFAULT = "LevelTitle";
const FString  GameConstants::LVL_TITLE_CANYON = "Canyon";
const FString  GameConstants::LVL_TITLE_HUB = "Hub";
const FString  GameConstants::LVL_TITLE_FOREST = "Forest";
const FString  GameConstants::LVL_TITLE_ICE = "Ice";
const FString  GameConstants::LVL_TITLE_VOLCANO = "Volcano";

//Game
const int  GameConstants::PICKUP_SCORE_MAX_CANYON = 60;
const int  GameConstants::PICKUP_SCORE_MAX_FOREST = 140;
const int  GameConstants::PICKUP_SCORE_MAX_ICE = 3;
const int  GameConstants::PICKUP_SCORE_MAX_VOLCANO = 4;
#pragma endregion


GameConstants::GameConstants()
{
}

GameConstants::~GameConstants()
{
}
