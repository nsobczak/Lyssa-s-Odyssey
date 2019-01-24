// Fill out your copyright notice in the Description page of Project Settings.

#include "GameConstants.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"

const FColor GameConstants::ColorTextLightGray = FColor::FromHex("#606060FF");

#pragma region Defaults Keys
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
const int CanyonPickupAmount = 1;
const int ForestPickupAmount = 2;
const int IcePickupAmount = 3;
const int VolcanoPickupAmount = 4;
#pragma endregion

GameConstants::GameConstants()
{
}

GameConstants::~GameConstants()
{
}
