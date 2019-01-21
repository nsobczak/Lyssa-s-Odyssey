// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

#pragma region Defaults Keys
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

};
