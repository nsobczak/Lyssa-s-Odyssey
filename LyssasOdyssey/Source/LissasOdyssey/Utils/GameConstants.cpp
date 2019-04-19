// Fill out your copyright notice in the Description page of Project Settings.

#include "GameConstants.h"

const FColor UGameConstants::COLOR_TEXT_LIGHT_GRAY = FColor::FromHex("#606060FF");

#pragma region Game Settings

#pragma region Defaults general settings
const ELanguages UGameConstants::DEF_GENERAL_CURRENT_LANGUAGE = ELanguages::en;

const ETextSpeed UGameConstants::DEF_GENERAL_CURRENT_TEXT_SPEED = ETextSpeed::Normal;
const float UGameConstants::GENERAL_TEXT_SPEED_SLOW = 0.1f;
const float UGameConstants::GENERAL_TEXT_SPEED_NORMAL = 0.05f;
const float UGameConstants::GENERAL_TEXT_SPEED_FAST = 0.01f;
#pragma endregion

#pragma region Defaults graphics settings
const bool UGameConstants::DEF_GRAPHICAL_SHOW_FPS = false;
const bool UGameConstants::DEF_GRAPHICAL_IS_FULL_SCREEN = false;
const int32 UGameConstants::DEF_GRAPHICAL_GRAPHICAL_IDX = 3;
const int32 UGameConstants::DEF_GRAPHICAL_PP_IDX = 3;
const int32 UGameConstants::DEF_GRAPHICAL_AA_IDX = 3;
const int32 UGameConstants::DEF_GRAPHICAL_SHADOW_IDX = 3;
const int32 UGameConstants::DEF_GRAPHICAL_FPS_IDX = 2;
const int32 UGameConstants::DEF_GRAPHICAL_RES_IDX = 3;
#pragma endregion

#pragma region Defaults audio settings
const float UGameConstants::DEF_AUDIO_MASTER_VOL_VAL = 1.0f;
const float UGameConstants::DEF_AUDIO_MUSIC_VOL_VAL = 1.0f;
const float UGameConstants::DEF_AUDIO_EFFECT_VOL_VAL = 1.0f;
#pragma endregion

#pragma region Defaults keybind settings
const bool UGameConstants::DEF_USE_GAMEPAD = true;

////GamePad
//const FKey UGameConstants::DEF_GPKEY_MOVE_VERTICAL = EKeys::Gamepad_LeftY;
//const FKey UGameConstants::DEF_GPKEY_MOVE_HORIZONTAL = EKeys::Gamepad_LeftX;
//const FKey UGameConstants::DEF_GPKEY_FMOVE_VERTICAL = EKeys::Gamepad_RightY;
//const FKey UGameConstants::DEF_GPKEY_FMOVE_HORIZONTAL = EKeys::Gamepad_RightX;
//
//const FKey UGameConstants::DEF_GPKEY_ACROSS = EKeys::Gamepad_FaceButton_Bottom;
//const FKey UGameConstants::DEF_GPKEY_ATRIANGLE = EKeys::Gamepad_FaceButton_Right;
//const FKey UGameConstants::DEF_GPKEY_ASTART = EKeys::Gamepad_Special_Right;
//
////Keyboard
//const FKey UGameConstants::DEF_KKEY_MOVE_UP = EKeys::W;
//const FKey UGameConstants::DEF_KKEY_MOVE_RIGHT = EKeys::D;
//const FKey UGameConstants::DEF_KKEY_MOVE_DOWN = EKeys::S;
//const FKey UGameConstants::DEF_KKEY_MOVE_LEFT = EKeys::A;
//
//const FKey UGameConstants::DEF_KKEY_ACROSS = EKeys::E;
//const FKey UGameConstants::DEF_KKEY_ATRIANGLE = EKeys::R;
//const FKey UGameConstants::DEF_KKEY_ASTART = EKeys::T;
#pragma endregion

#pragma endregion

#pragma region Levels
//level map titles
const FName UGameConstants::LVL_MAP_TITLE_HUB = "Level_Hub";
const FName UGameConstants::LVL_MAP_TITLE_CANYON = "Level_Canyon_tuto";
const FName UGameConstants::LVL_MAP_TITLE_FOREST = "Level_Forest";
const FName UGameConstants::LVL_MAP_TITLE_ICE = "Level_Ice";
const FName UGameConstants::LVL_MAP_TITLE_VOLCANO = "Level_Volcano";
const FName UGameConstants::LVL_MAP_TITLE_PLAYGROUND = "Level_Playground";

//Game
const int UGameConstants::PICKUP_SCORE_MAX_CANYON = 60;
const int UGameConstants::PICKUP_SCORE_MAX_HUB = 0;
const int UGameConstants::PICKUP_SCORE_MAX_FOREST = 140;
const int UGameConstants::PICKUP_SCORE_MAX_ICE = 0;
const int UGameConstants::PICKUP_SCORE_MAX_VOLCANO = 144;
#pragma endregion

const float UGameConstants::FINAL_GATE_SCORE_PERCENTAGE = 0.75f;
const int UGameConstants::FROTATION_VALUE_COUNT = 20;


UGameConstants::UGameConstants()
{
}
