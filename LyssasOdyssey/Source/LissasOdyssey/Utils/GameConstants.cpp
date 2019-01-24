// Fill out your copyright notice in the Description page of Project Settings.

#include "GameConstants.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"

const FColor GameConstants::ColorTextLightGray = FColor::FromHex("#606060FF");

#pragma region Game Settings

#pragma region Defaults general settings
const ELanguages GameConstants::DefaultGeneral_CurrentLanguage = ELanguages::en;
#pragma endregion

#pragma region Defaults graphics settings
const bool GameConstants::DefaultGraphical_ShowFPS = false;
const bool GameConstants::DefaultGraphical_IsFullScreen = false;
const int32 GameConstants::DefaultGraphical_GraphicalIndex = 3;
const int32 GameConstants::DefaultGraphical_PPIndex = 3;
const int32 GameConstants::DefaultGraphical_AAIndex = 3;
const int32 GameConstants::DefaultGraphical_ShadowIndex = 3;
const int32 GameConstants::DefaultGraphical_FPSIndex = 2;
const int32 GameConstants::DefaultGraphical_ResIndex = 3;
#pragma endregion

#pragma region Defaults audio settings
const float GameConstants::DefaultAudio_MasterVolumeSliderValue = 1.0f;
const float GameConstants::DefaultAudio_MusicVolumeSliderValue = 1.0f;
const float GameConstants::DefaultAudio_EffectVolumeSliderValue = 1.0f;
#pragma endregion

#pragma region Defaults keybind settings
const bool GameConstants::UseGamePad = true;

//GamePad
const FKey GameConstants::DefaultGPKey_MoveVertical = EKeys::Gamepad_LeftY;
const FKey GameConstants::DefaultGPKey_MoveHorizontal = EKeys::Gamepad_LeftX;
const FKey GameConstants::DefaultGPKey_FMoveVertical = EKeys::Gamepad_RightY;
const FKey GameConstants::DefaultGPKey_FMoveHorizontal = EKeys::Gamepad_RightX;

const FKey GameConstants::DefaultGPKey_ACross = EKeys::Gamepad_FaceButton_Bottom;
const FKey GameConstants::DefaultGPKey_ATriangle = EKeys::Gamepad_FaceButton_Right;
const FKey GameConstants::DefaultGPKey_AStart = EKeys::Gamepad_Special_Right;

//Keyboard
const FKey GameConstants::DefaultKKey_MoveUp = EKeys::W;
const FKey GameConstants::DefaultKKey_MoveRight = EKeys::D;
const FKey GameConstants::DefaultKKey_MoveDown = EKeys::S;
const FKey GameConstants::DefaultKKey_MoveLeft = EKeys::A;

const FKey GameConstants::DefaultKKey_ACross = EKeys::E;
const FKey GameConstants::DefaultKKey_ATriangle = EKeys::R;
const FKey GameConstants::DefaultKKey_AStart = EKeys::T;
#pragma endregion

#pragma region Game
const int GameConstants::CanyonPickupAmount = 1;
const int GameConstants::ForestPickupAmount = 2;
const int GameConstants::IcePickupAmount = 3;
const int GameConstants::VolcanoPickupAmount = 4;
#pragma endregion

#pragma endregion

GameConstants::GameConstants()
{
}

GameConstants::~GameConstants()
{
}
