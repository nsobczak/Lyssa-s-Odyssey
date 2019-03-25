// Fill out your copyright notice in the Description page of Project Settings.

#include "GameConstants.h"

const FColor GameConstants::COLOR_TEXT_LIGHT_GRAY = FColor::FromHex("#606060FF");

#pragma region Game Settings

#pragma region Defaults general settings
const ELanguages GameConstants::DEF_GENERAL_CURRENT_LANGUAGE = ELanguages::en;

const ETextSpeed GameConstants::DEF_GENERAL_CURRENT_TEXT_SPEED = ETextSpeed::Normal;
const float GameConstants::GENERAL_TEXT_SPEED_SLOW = 0.1f;
const float GameConstants::GENERAL_TEXT_SPEED_NORMAL = 0.05f;
const float GameConstants::GENERAL_TEXT_SPEED_FAST = 0.01f;
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

#pragma region Levels
//level titles
const FString  GameConstants::LVL_TITLE_DEFAULT = "LevelTitle";
const FString  GameConstants::LVL_TITLE_CANYON = "Canyon";
const FString  GameConstants::LVL_TITLE_HUB = "Hub";
const FString  GameConstants::LVL_TITLE_FOREST = "Forest";
const FString  GameConstants::LVL_TITLE_ICE = "Ice";
const FString  GameConstants::LVL_TITLE_VOLCANO = "Volcano";
const FString GameConstants::LVL_TITLE_PLAYGROUND = "Playground";

//level map titles
const FName GameConstants::LVL_MAP_TITLE_HUB = "Level_Hub";
const FName GameConstants::LVL_MAP_TITLE_CANYON = "Level_Canyon_tuto";
const FName GameConstants::LVL_MAP_TITLE_FOREST = "Level_Forest";
const FName GameConstants::LVL_MAP_TITLE_ICE = "Level_Ice";
const FName GameConstants::LVL_MAP_TITLE_VOLCANO = "Level_Volcano";
const FName GameConstants::LVL_MAP_TITLE_PLAYGROUND = "Level_Playground";

//Game
const int  GameConstants::PICKUP_SCORE_MAX_CANYON = 60;
const int  GameConstants::PICKUP_SCORE_MAX_FOREST = 140;
const int  GameConstants::PICKUP_SCORE_MAX_ICE = 0;
const int  GameConstants::PICKUP_SCORE_MAX_VOLCANO = 144;
#pragma endregion

const float GameConstants::FINAL_GATE_SCORE_PERCENTAGE = 0.75f;
const int GameConstants::FROTATION_VALUE_COUNT = 20;


GameConstants::GameConstants()
{
}

GameConstants::~GameConstants()
{
}
