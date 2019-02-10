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

UENUM(BlueprintType)
enum LineBoolSwitch { Key_Device, ShowMinimap, Graphical_FullScreen, Graphical_ShowFPS };

#pragma endregion

#pragma region Game

UENUM(BlueprintType)
enum LevelLabels { LevelTitle, Canyon, Hub, Forest, Ice, Volcano, Playground };

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
