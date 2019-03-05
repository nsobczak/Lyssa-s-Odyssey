#include "Lyssa.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"

#include "GameModes/LevelGameMode.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "ConstructorHelpers.h"
#include "Characters/CharacterActors/Shot.h"

#include "Pickups/Pickup.h"
#include "Pickups/PickupLife.h"
#include "Pickups/PickupScore.h"
#include "Pickups/PickupKey.h"


#pragma region Initialization
//____________________________________________________________________________________

// Sets default values
ALyssa::ALyssa(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	RootComponent = GetCapsuleComponent();
	PrimaryActorTick.bTickEvenWhenPaused = true;

	//create the static mesh component
	LyssaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LyssaMesh"));
	LyssaMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	////create Fylgja
	//Fylgja = CreateDefaultSubobject<AFylgja>(TEXT("Fylgja"));
	//Fylgja->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// === CameraComponent === 
	MainCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("MainCamera"));
	MainCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MainCamera->RelativeLocation = FVector(-400, 0, 1000.0f);// Position the camera
	MainCamera->RelativeRotation.Pitch = 290.0f;
	MainCamera->bUsePawnControlRotation = false; // Allow the pawn to control rotation.

	ScorePickupAmount = 0;
}

// Called when the game starts or when spawned
void ALyssa::BeginPlay()
{
	Super::BeginPlay();

	initialPosZValue = GetActorLocation().Z;

	// === Fylgja === 
	TArray<AActor*> Comps;
	GetAllChildActors(Comps);
	for (size_t i = 0; i < Comps.Num(); i++)
	{
		if (Comps[i]->IsA(AFylgja::StaticClass()))
			Fylgja = (AFylgja*)Comps[i];
	}
}


// Called every frame
void ALyssa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotation();

	CollectPickups();
}

AFylgja* ALyssa::GetFylgja() const
{
	return Fylgja;
}
//____________________________________________________________________________________
#pragma endregion


#pragma region Input
//____________________________________________________________________________________

void ALyssa::WaitForLoadCompletionAndAssignKeys(ALevelGameMode* currentGameMode, UInputComponent* playerInputComponent, float checkInterval)
{
	UE_LOG(LogTemp, Log, TEXT("waiting for begin function completion | IsBeginFunctionCompleted = %s"), (currentGameMode->GetIsBeginFunctionCompleted() ? TEXT("True") : TEXT("False")));

	if (currentGameMode->GetIsBeginFunctionCompleted())
	{
		//TODO: debind all before reassigning
		//playerInputComponent->BindAxis("MoveUp", this, &ALyssa::MoveUp);
		//playerInputComponent->BindAxis("MoveRight", this, &ALyssa::MoveRight);

		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapKeys;

		//-Gamepad-
		TMapKeys = currentGameMode->TMapGamepadKeys;
		UE_LOG(LogTemp, Log, TEXT("Gamepad keys before binding: %s %s %s %s %s %s %s"), *(TMapKeys.FindRef(PlayerActionLabel::MoveUp).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveDown).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::MoveLeft).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveRight).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::ACross).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::ATriangle).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::AStart).ToString()));
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveUp), this, &ALyssa::MoveUp);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveDown), this, &ALyssa::MoveDown);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveLeft), this, &ALyssa::MoveLeft);
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveRight), this, &ALyssa::MoveRight);
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::FMoveUp), this, &ALyssa::MoveFUp);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::FMoveDown), this, &ALyssa::MoveFDown);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::FMoveLeft), this, &ALyssa::MoveFLeft);
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::FMoveRight), this, &ALyssa::MoveFRight);
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ACross), EInputEvent::IE_Released, this, &ALyssa::ActionAccept).bExecuteWhenPaused = true;
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ATriangle), EInputEvent::IE_Released, this, &ALyssa::ActionReturn).bExecuteWhenPaused = true;
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::AStart), EInputEvent::IE_Released, this, &ALyssa::PauseGame).bExecuteWhenPaused = true;

		//-Keyboard-
		TMapKeys = currentGameMode->TMapKeyboardKeys;
		UE_LOG(LogTemp, Log, TEXT("keyboard keys before binding: %s %s %s %s %s %s %s"), *(TMapKeys.FindRef(PlayerActionLabel::MoveUp).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveDown).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::MoveLeft).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveRight).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::ACross).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::ATriangle).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::AStart).ToString()));

		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveUp), this, &ALyssa::MoveUp);
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveDown), this, &ALyssa::MoveDown);
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveLeft), this, &ALyssa::MoveLeft);
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveRight), this, &ALyssa::MoveRight);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::FMoveUp), this, &ALyssa::MoveFUp);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::FMoveDown), this, &ALyssa::MoveFDown);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::FMoveLeft), this, &ALyssa::MoveFLeft);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::FMoveRight), this, &ALyssa::MoveFRight);
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ACross), EInputEvent::IE_Released, this, &ALyssa::ActionAccept).bExecuteWhenPaused = true;
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ATriangle), EInputEvent::IE_Released, this, &ALyssa::ActionReturn).bExecuteWhenPaused = true;
		playerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::AStart), EInputEvent::IE_Released, this, &ALyssa::PauseGame).bExecuteWhenPaused = true;
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
void ALyssa::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// === GameMode ===
	ALevelGameMode* CurrentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (CurrentGameMode)
	{
		//check for CurrentGameMode begin completion, wait if not completed

		//UE_LOG(LogTemp, Log, TEXT("before waiting for begin function completion"));
		WaitForLoadCompletionAndAssignKeys(CurrentGameMode, PlayerInputComponent, 0.05f);
		//UE_LOG(LogTemp, Log, TEXT("after waiting for begin function completion"));
	}
}

#pragma region Lyssa
//_______________
void ALyssa::MoveUp(float value)
{
	if (Controller != NULL && value != 0)
	{
		if (value * value > 0.5f * 0.5f)
			OnUpDelegate.Broadcast();

		// find out which way is up
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get direction vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, value);

		// Set Character's rotation
		topKeyValue = value;
		//UE_LOG(LogTemp, Log, TEXT("topKeyValue = %f"), topKeyValue);
	}
}

void ALyssa::MoveDown(float value)
{
	if (Controller != NULL && value != 0)
	{
		if (value * value > 0.5f * 0.5f)
			OnDownDelegate.Broadcast();

		// find out which way is up
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get direction vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, -value);

		// Set Character's rotation
		downKeyValue = value;
		//UE_LOG(LogTemp, Log, TEXT("downKeyValue = %f"), downKeyValue);
	}
}

void ALyssa::MoveRight(float value)
{
	if (Controller != NULL && value != 0)
	{
		if (value * value > 0.5f * 0.5f)
			OnRightDelegate.Broadcast();

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, value);

		// Set Character's rotation
		rightKeyValue = value;
		//UE_LOG(LogTemp, Log, TEXT("rightKeyValue = %f"), rightKeyValue);
	}
}

void ALyssa::MoveLeft(float value)
{
	if (Controller != NULL && value != 0)
	{
		if (value * value > 0.5f * 0.5f)
			OnLeftDelegate.Broadcast();

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, -value);

		// Set Character's rotation
		leftKeyValue = value;
		//UE_LOG(LogTemp, Log, TEXT("leftKeyValue = %f"), leftKeyValue);
	}
}

void ALyssa::UpdateRotation()
{
	/*UE_LOG(LogTemp, Log, TEXT("UpdateRotation - values: topKeyValue = %f | downKeyValue = %f | rightKeyValue = %f | leftKeyValue = %f"),
		topKeyValue, downKeyValue, rightKeyValue, leftKeyValue);*/

		//1 value
	if (leftKeyValue != 0 && topKeyValue == 0 && rightKeyValue == 0 && downKeyValue == 0)
	{
		if (leftKeyValue > 0)
			rotationAngle = 270.0f;
		else
			rotationAngle = 90.0f;
		//UE_LOG(LogTemp, Log, TEXT("rotationAngle = %f"), rotationAngle);
	}
	else if (leftKeyValue == 0 && topKeyValue != 0 && rightKeyValue == 0 && downKeyValue == 0)
	{
		if (topKeyValue > 0)
			rotationAngle = 0.0f;
		else
			rotationAngle = 180.0f;
		//UE_LOG(LogTemp, Log, TEXT("rotationAngle = %f"), rotationAngle);
	}
	else if (leftKeyValue == 0 && topKeyValue == 0 && rightKeyValue != 0 && downKeyValue == 0)
	{
		if (rightKeyValue > 0)
			rotationAngle = 90.0f;
		else
			rotationAngle = 270.0f;
		//UE_LOG(LogTemp, Log, TEXT("rotationAngle = %f"), rotationAngle);
	}
	else if (leftKeyValue == 0 && topKeyValue == 0 && rightKeyValue == 0 && downKeyValue != 0)
	{
		if (downKeyValue > 0)
			rotationAngle = 180.0f;
		else
			rotationAngle = 0.0f;
		//UE_LOG(LogTemp, Log, TEXT("rotationAngle = %f"), rotationAngle);
	}

	//2 values
	else if (leftKeyValue == 0 && topKeyValue != 0 && rightKeyValue != 0 && downKeyValue == 0)
	{
		rotationAngle = atan2f(rightKeyValue, topKeyValue) * 180 / PI;// 45.0f;
		//UE_LOG(LogTemp, Log, TEXT("rotationAngle = %f"), rotationAngle);
	}
	else if (leftKeyValue == 0 && topKeyValue == 0 && rightKeyValue != 0 && downKeyValue != 0)
	{
		rotationAngle = 90.0f + atan2f(downKeyValue, rightKeyValue) * 180 / PI;// 135.0f;
		//UE_LOG(LogTemp, Log, TEXT("rotationAngle = %f"), rotationAngle);
	}
	else if (leftKeyValue != 0 && topKeyValue == 0 && rightKeyValue == 0 && downKeyValue != 0)
	{
		rotationAngle = 180.0f + atan2f(leftKeyValue, downKeyValue) * 180 / PI; //225.0f;
		//UE_LOG(LogTemp, Log, TEXT("rotationAngle = %f"), rotationAngle);
	}
	else if (leftKeyValue != 0 && topKeyValue != 0 && rightKeyValue == 0 && downKeyValue == 0)
	{
		rotationAngle = 270.0f + atan2f(topKeyValue, leftKeyValue) * 180 / PI; //315.0f;
		//UE_LOG(LogTemp, Log, TEXT("rotationAngle = %f"), rotationAngle);
	}

	this->GetMesh()->SetRelativeRotation(FRotator(0, rotationAngle, 0));
	leftKeyValue = 0; topKeyValue = 0; rightKeyValue = 0; downKeyValue = 0;
}
//_______________
#pragma endregion


#pragma region Fylgja
//_______________
void ALyssa::MoveFUp(float value)
{
	if (value != 0)
	{
		// Set Character's rotation
		this->Fylgja->fTopKeyValue = value;
		//UE_LOG(LogTemp, Log, TEXT("ftopKeyValue = %f"), this->Fylgja->fTopKeyValue);
	}
}

void ALyssa::MoveFDown(float value)
{
	if (value != 0)
	{
		// Set Character's rotation
		this->Fylgja->fDownKeyValue = value;
		//UE_LOG(LogTemp, Log, TEXT("fdownKeyValue = %f"), this->Fylgja->fDownKeyValue);
	}
}

void ALyssa::MoveFRight(float value)
{
	if (value != 0)
	{
		// Set Character's rotation
		this->Fylgja->fRightKeyValue = value;
		//UE_LOG(LogTemp, Log, TEXT("frightKeyValue = %f"), this->Fylgja->fRightKeyValue);
	}
}

void ALyssa::MoveFLeft(float value)
{
	if (value != 0)
	{
		// Set Character's rotation
		this->Fylgja->fLeftKeyValue = value;
		//UE_LOG(LogTemp, Log, TEXT("fleftKeyValue = %f"), this->Fylgja->fLeftKeyValue);
	}
}
//_______________
#pragma endregion


void ALyssa::ActionAccept()
{
	//UE_LOG(LogTemp, Log, TEXT("ActionAccept"));
	OnAcceptDelegate.Broadcast();
}

void ALyssa::ActionReturn()
{
	//UE_LOG(LogTemp, Log, TEXT("ActionReturn"));
	OnReturnDelegate.Broadcast();
}

void ALyssa::PauseGame()
{
	// === GameMode ===
	ALevelGameMode* CurrentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (CurrentGameMode && CurrentGameMode->IsPauseAllowed)//IsPauseAllowed
	{
		//UE_LOG(LogTemp, Warning, TEXT("Pause called"));
		CurrentGameMode->SetCurrentState(ELevelPlayState::EPause);
	}
}
//____________________________________________________________________________________
#pragma endregion


#pragma region Pickup key
//____________________________________________________________________________________
TArray<TEnumAsByte<KeyLockNature>> ALyssa::GetKeyPickups()
{
	return this->KeyPickups;
}

void ALyssa::AddKeyPickup(TEnumAsByte<KeyLockNature> pickupNatureToAdd)
{
	this->KeyPickups.Add(pickupNatureToAdd);
}

bool ALyssa::RemoveKeyPickup(TEnumAsByte<KeyLockNature> pickupNatureToRemove)
{
	if (this->KeyPickups.Contains(pickupNatureToRemove))
	{
		this->KeyPickups.Remove(pickupNatureToRemove);
		return true;
	}

	return false;
}
//____________________________________________________________________________________
#pragma endregion



#pragma region Pickup score
//____________________________________________________________________________________
int ALyssa::GetCurrentScorePickupAmount()
{
	return ScorePickupAmount;
}

void ALyssa::UpdateScorePickupAmount(int amountChange)
{
	ScorePickupAmount += amountChange;
	if (ScorePickupAmount < 0)
	{
		ScorePickupAmount = 0;
	}
}

TMap<TEnumAsByte<LevelLabels>, int>  ALyssa::GetTMapPlayerPickupAmountByLevel()
{
	return this->TMapPlayerPickupAmountByLevel;
}

void ALyssa::SetTMapPlayerPickupAmountByLevel(TMap<TEnumAsByte<LevelLabels>, int> newTMap)
{
	this->TMapPlayerPickupAmountByLevel = newTMap;
}

int ALyssa::GetPlayerPickupTotalScore()
{
	int result = 0;
	TArray<TEnumAsByte<LevelLabels>> levelKeys;
	this->TMapPlayerPickupAmountByLevel.GetKeys(levelKeys);
	for (size_t i = 0; i < levelKeys.Num(); ++i)
	{
		result += this->TMapPlayerPickupAmountByLevel.FindRef(levelKeys[i]);
	}
	return result;
}

void ALyssa::EmplaceTMapPlayerPickupAmountByLevel(LevelLabels LevelToChange, int newAmount)
{
	if (this->TMapPlayerPickupAmountByLevel.Contains(LevelToChange))
	{
		this->TMapPlayerPickupAmountByLevel.Emplace(LevelToChange, newAmount);
	}
}

void ALyssa::UpdateTMapPlayerPickupAmountByLevel(LevelLabels LevelToChange, int amountChange)
{
	if (this->TMapPlayerPickupAmountByLevel.Contains(LevelToChange))
	{
		this->TMapPlayerPickupAmountByLevel.Emplace(
			LevelToChange, this->TMapPlayerPickupAmountByLevel.FindRef(LevelToChange) + amountChange);
	}
}

void ALyssa::ResetTMapPlayerPickupAmountByLevel()
{
	this->TMapPlayerPickupAmountByLevel.Reset();
	TMapPlayerPickupAmountByLevel.Emplace(LevelLabels::Canyon, 0);
	TMapPlayerPickupAmountByLevel.Emplace(LevelLabels::Forest, 0);
	TMapPlayerPickupAmountByLevel.Emplace(LevelLabels::Ice, 0);
	TMapPlayerPickupAmountByLevel.Emplace(LevelLabels::Volcano, 0);
	this->IsInitialized_TMapPlayerPickupByLevel = true;
}

void ALyssa::CollectPickups()
{
	//get overlaping actors and store them in an array
	TArray<AActor*> collectedActors;
	LyssaMesh->GetOverlappingActors(collectedActors);

	//for each actor we collected
	for (int32 iCollected = 0; iCollected < collectedActors.Num(); ++iCollected)
	{
		//cast the actor to apickup
		APickup* const castedPickup = Cast<APickup>(collectedActors[iCollected]);
		//if cast succesful and pickup is valid and active
		if (castedPickup && !castedPickup->IsPendingKill() && castedPickup->IsActive())
		{
			//call the pickup's wasCollected function
			castedPickup->WasCollected();

			//check to see pickup nature and apply effect
			APickupLife* const castedLife = Cast<APickupLife>(castedPickup);
			APickupScore* const castedScore = Cast<APickupScore>(castedPickup);
			APickupKey* const castedKey = Cast<APickupKey>(castedPickup);

			if (castedLife)
				UpdateLife(+castedLife->GetLifeAmount());
			else if (castedScore)
				UpdateScorePickupAmount(+castedScore->GetScoreAmount());
			else if (castedKey)
				AddKeyPickup(castedKey->GetKeyNature());

			//deactivate the pickup
			castedPickup->SetActive(false);
		}
	}
}
//____________________________________________________________________________________
#pragma endregion
