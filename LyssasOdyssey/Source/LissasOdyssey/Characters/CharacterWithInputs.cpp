#include "CharacterWithInputs.h"

#include "GameModes/MainGameMode.h"
#include "Utils/GameEnums.h"

#include "Engine/Public/TimerManager.h"
#include "Engine/World.h"


#pragma region Initialization
//____________________________________________________________________________________

// Sets default values
ACharacterWithInputs::ACharacterWithInputs(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bTickEvenWhenPaused = true;
}

// Called when the game starts or when spawned
void ACharacterWithInputs::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacterWithInputs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//____________________________________________________________________________________
#pragma endregion


#pragma region Inputs
//____________________________________________________________________________________

void ACharacterWithInputs::WaitForLoadCompletionAndAssignKeys(AMainGameMode* currentGameMode, UInputComponent* playerInputComponent, float checkInterval)
{
	UE_LOG(LogTemp, Log, TEXT("waiting for begin function completion | IsBeginFunctionCompleted = %s"), (currentGameMode->GetIsBeginFunctionCompleted() ? TEXT("True") : TEXT("False")));

	if (currentGameMode->GetIsBeginFunctionCompleted())
	{
		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapKeys;

		//-Gamepad-
		TMapKeys = currentGameMode->TMapGamepadKeys;
		UE_LOG(LogTemp, Log, TEXT("Gamepad keys before binding: %s %s %s %s %s %s %s"), *(TMapKeys.FindRef(PlayerActionLabel::MoveUp).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveDown).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::MoveLeft).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveRight).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::ACross).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::ATriangle).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::AStart).ToString()));

		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveUp), this, &ACharacterWithInputs::MoveUp).bExecuteWhenPaused = true;
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveDown), this, &ACharacterWithInputs::MoveDown);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveLeft), this, &ACharacterWithInputs::MoveLeft);
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveRight), this, &ACharacterWithInputs::MoveRight).bExecuteWhenPaused = true;
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ACross), EInputEvent::IE_Released, this, &ACharacterWithInputs::ActionAccept).bExecuteWhenPaused = true;
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ATriangle), EInputEvent::IE_Released, this, &ACharacterWithInputs::ActionReturn).bExecuteWhenPaused = true;

		//-Keyboard-
		TMapKeys = currentGameMode->TMapKeyboardKeys;
		UE_LOG(LogTemp, Log, TEXT("keyboard keys before binding: %s %s %s %s %s %s %s"), *(TMapKeys.FindRef(PlayerActionLabel::MoveUp).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveDown).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::MoveLeft).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveRight).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::ACross).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::ATriangle).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::AStart).ToString()));

		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveUp), this, &ACharacterWithInputs::MoveUp).bExecuteWhenPaused = true;
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveDown), this, &ACharacterWithInputs::MoveDown).bExecuteWhenPaused = true;
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveLeft), this, &ACharacterWithInputs::MoveLeft).bExecuteWhenPaused = true;
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveRight), this, &ACharacterWithInputs::MoveRight).bExecuteWhenPaused = true;
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ACross), EInputEvent::IE_Released, this, &ACharacterWithInputs::ActionAccept).bExecuteWhenPaused = true;
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ATriangle), EInputEvent::IE_Released, this, &ACharacterWithInputs::ActionReturn).bExecuteWhenPaused = true;
	}
	else
	{
		//delay
		FTimerHandle TimerHandle; // Handle to manage the timer
		FTimerDelegate TimerDel; //Bind function with parameters
		TimerDel.BindUFunction(this, FName("WaitForLoadCompletionAndAssignKeys"), currentGameMode, playerInputComponent, checkInterval);
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, checkInterval, false); //0.1f as default parameter
	}
}

// Called to bind functionality to input
void ACharacterWithInputs::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// === GameMode ===
	AMainGameMode* CurrentGameMode = (AMainGameMode*)GetWorld()->GetAuthGameMode();
	if (CurrentGameMode)
	{
		//check for CurrentGameMode begin completion, wait if not completed

		//UE_LOG(LogTemp, Log, TEXT("before waiting for begin function completion"));
		WaitForLoadCompletionAndAssignKeys(CurrentGameMode, PlayerInputComponent, 0.1f);
		//UE_LOG(LogTemp, Log, TEXT("after waiting for begin function completion"));
	}
}

#pragma region Broadcasts
//_______________
void ACharacterWithInputs::MoveUp(float value)
{
	if (Controller != NULL && value != 0)
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("MoveUp with value of %f"), value);

		if (value > 0.5f)
		{
			OnUpDelegate.Broadcast();
		}
		else if (value < -0.5f)
		{
			OnDownDelegate.Broadcast();
		}
	}

}

void ACharacterWithInputs::MoveDown(float value)
{
	if (Controller != NULL && value != 0)
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("MoveDown pressed with value of %f"), value);

		if (value > 0.5f)
		{
			OnDownDelegate.Broadcast();
		}
		else if (value < -0.5f)
		{
			OnUpDelegate.Broadcast();
		}
	}
}

void ACharacterWithInputs::MoveRight(float value)
{
	if (Controller != NULL && value != 0)
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("MoveRight pressed with value of %f"), value);

		if (value > 0.5f)
		{
			OnRightDelegate.Broadcast();
		}
		else if (value < -0.5f)
		{
			OnLeftDelegate.Broadcast();
		}
	}
}

void ACharacterWithInputs::MoveLeft(float value)
{
	if (Controller != NULL && value != 0)
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("MoveRight pressed with value of %f"), value);

		if (value > 0.5f)
		{
			OnLeftDelegate.Broadcast();
		}
		else if (value < -0.5f)
		{
			OnRightDelegate.Broadcast();
		}
	}
}

void ACharacterWithInputs::ActionAccept()
{
	OnAcceptDelegate.Broadcast();
	if (DEBUG) UE_LOG(LogTemp, Log, TEXT("ActionAccept"));
}

void ACharacterWithInputs::ActionReturn()
{
	OnReturnDelegate.Broadcast();
	if (DEBUG) UE_LOG(LogTemp, Log, TEXT("ActionReturn"));
}
//_______________
#pragma endregion

//____________________________________________________________________________________
#pragma endregion

