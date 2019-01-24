// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.generated.h"

#pragma region Settings
UENUM(BlueprintType)	//"BlueprintType" is essential specifier
enum class ELanguages : uint8 { en, fr, count };

UENUM(BlueprintType)
enum GraphicLabel { Graphical, PP, AA, Shadow, FPS, Res };

UENUM(BlueprintType)
enum PlayerActionLabel
{
	MoveUp, MoveRight, MoveDown, MoveLeft, ATriangle, ACircle, ACross, ARectangle, AStart, ASelect,
	FMoveUp, FMoveRight, FMoveDown, FMoveLeft, AL1, AR1, AL2, AR2
};

UENUM(BlueprintType)
enum DeviceName { Keyboard, Gamepad };

UENUM(BlueprintType)
enum SoundLabel { Music, Effect };

#pragma endregion

#pragma region Game

UENUM(BlueprintType)
enum LevelLabels { Canyon, Hub, Forest, Ice, Volcano };

#pragma endregion

/**
 * 
 */
class LISSASODYSSEY_API GameEnums
{
public:
	GameEnums();
	~GameEnums();
};
