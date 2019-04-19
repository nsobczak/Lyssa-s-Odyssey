#include "Lyssa.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "ConstructorHelpers.h"

#include "GameModes/LevelGameMode.h"
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
	PrimaryActorTick.bTickEvenWhenPaused = false;

	LyssaSKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LyssaSKMesh"));
	LyssaSKMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

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


void ALyssa::UpdateSKMeshBodyColor()
{
	if (ColorsArray.Num() > 0)
	{
		UMaterialInterface* lyssaBodyMat = ColorsArray[BodyMatIdx].Material;
		LyssaSKMesh->SetMaterial(0, lyssaBodyMat);
	}
}

void ALyssa::UpdateSKMeshShapeColor()
{
	if (ColorsArray.Num() > 0)
	{
		UMaterialInterface* lyssaShapeMat = ColorsArray[ShapeMatIdx].Material;
		LyssaSKMesh->SetMaterial(1, lyssaShapeMat);
	}
}

void ALyssa::UpdateSKMeshColors()
{
	UpdateSKMeshBodyColor();
	UpdateSKMeshShapeColor();
}

void ALyssa::InitIndexesValueFromGameMode()
{
	this->BodyMatIdx = CurrentMainGameMode->LyssaBodyMatIdx;
	this->ShapeMatIdx = CurrentMainGameMode->LyssaShapeMatIdx;
	UE_LOG(LogTemp, Log, TEXT("Initialized current lyssa color indexes value from CurrentMainGameMode: BodyMatIdx = %i, ShapeMatIdx = %i"),
		BodyMatIdx, ShapeMatIdx);
}

int ALyssa::GetBodyMatIdx() const
{
	return this->BodyMatIdx;
}

void ALyssa::SetBodyMatIdx(int idx)
{
	this->BodyMatIdx = idx;
	CurrentMainGameMode->LyssaBodyMatIdx = idx;
}

int ALyssa::GetShapeMatIdx() const
{
	return this->ShapeMatIdx;
}

void ALyssa::SetShapeMatIdx(int idx)
{
	this->ShapeMatIdx = idx;
	CurrentMainGameMode->LyssaShapeMatIdx = idx;
}

AFylgja* ALyssa::GetFylgja() const
{
	return Fylgja;
}

void ALyssa::PrintBodyMatIdx()
{
	UE_LOG(LogTemp, Log, TEXT("BodyMatIdx = %i"), this->BodyMatIdx);
}

void ALyssa::PrintShapeMatIdx()
{
	UE_LOG(LogTemp, Log, TEXT("ShapeMatIdx = %i"), this->ShapeMatIdx);
}

void ALyssa::DecreaseBodyMatIdx()
{
	SetBodyMatIdx(BodyMatIdx - 1);
	if (BodyMatIdx < 0)
	{
		SetBodyMatIdx(ColorsArray.Num() - 1);
	}

	UpdateSKMeshBodyColor();
}

void ALyssa::IncreaseBodyMatIdx()
{
	SetBodyMatIdx(BodyMatIdx + 1);
	if (BodyMatIdx > ColorsArray.Num() - 1)
	{
		SetBodyMatIdx(0);
	}

	UpdateSKMeshBodyColor();
}

void ALyssa::DecreaseShapeMatIdx()
{
	SetShapeMatIdx(ShapeMatIdx - 1);
	if (ShapeMatIdx < 0)
	{
		SetShapeMatIdx(ColorsArray.Num() - 1);
	}

	UpdateSKMeshShapeColor();
}

void ALyssa::IncreaseShapeMatIdx()
{
	SetShapeMatIdx(ShapeMatIdx + 1);
	if (ShapeMatIdx > ColorsArray.Num() - 1)
	{
		SetShapeMatIdx(0);
	}

	UpdateSKMeshShapeColor();
}

FStructMaterialWithName ALyssa::GetBodyStructMat() const
{
	return ColorsArray[BodyMatIdx];
}

FStructMaterialWithName ALyssa::GetShapeStructMat() const
{
	return ColorsArray[ShapeMatIdx];
}


void ALyssa::ColorInitialization()
{
	if (CurrentMainGameMode->GetIsBeginFunctionCompleted())
	{
		if (ColorDataTable)
		{
			ColorsArray.Reset();
			FString context;
			TArray<FStructMaterialWithName*> cArray;
			ColorDataTable->GetAllRows(context, cArray);
			for (size_t i = 0; i < cArray.Num(); ++i)
			{
				ColorsArray.Add(*(cArray[i]));
			}

			InitIndexesValueFromGameMode();
			UpdateSKMeshColors();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ColorDataTable is null"));
		}
	}
	else
	{
		//delay
		FTimerHandle TimerHandle; // Handle to manage the timer
		FTimerDelegate TimerDel; //Bind function with parameters
		TimerDel.BindUFunction(this, FName("ColorInitialization"));
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.2f, false); //0.1f as default parameter
	}
}

// Called when the game starts or when spawned
void ALyssa::BeginPlay()
{
	Super::BeginPlay();

	InitialPosZValue = GetActorLocation().Z;

	// === GameMode ===
	CurrentMainGameMode = (AMainGameMode*)GetWorld()->GetAuthGameMode();
	if (!CurrentMainGameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentMainGameMode is null"));
	}

	ColorInitialization();

	// === Fylgja === 
	TArray<AActor*> Comps;
	GetAllChildActors(Comps);
	for (size_t i = 0; i < Comps.Num(); i++)
	{
		if (Comps[i]->IsA(AFylgja::StaticClass()))
			Fylgja = (AFylgja*)Comps[i];
	}
}

//____________________________________________________________________________________
#pragma endregion


// Called every frame
void ALyssa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;


	UpdateRotation();

	if (IsMoving && Timer > DelayBeforeNotMoving)
	{
		IsMoving = false;
	}

	CollectPickups();
}


#pragma region Input
//____________________________________________________________________________________

void ALyssa::WaitForLoadCompletionAndAssignKeys(ALevelGameMode* currentGameMode, UInputComponent* playerInputComponent, float checkInterval)
{
	UE_LOG(LogTemp, Log, TEXT("waiting for begin function completion | IsBeginFunctionCompleted = %s"), (currentGameMode->GetIsBeginFunctionCompleted() ? TEXT("True") : TEXT("False")));

	if (currentGameMode->GetIsBeginFunctionCompleted())
	{
		//TODO: debind all before reassigning

		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapKeys;

		//-Gamepad-
		TMapKeys = currentGameMode->TMapGamepadKeys;
		UE_LOG(LogTemp, Log, TEXT("Gamepad keys before binding: %s %s %s %s %s %s %s"), *(TMapKeys.FindRef(PlayerActionLabel::MoveUp).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveDown).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::MoveLeft).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveRight).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::ACross).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::ATriangle).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::AStart).ToString()));

		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveUp), this, &ALyssa::MoveUp).bExecuteWhenPaused = true;
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveDown), this, &ALyssa::MoveDown);
		//playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveLeft), this, &ALyssa::MoveLeft);
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveRight), this, &ALyssa::MoveRight).bExecuteWhenPaused = true;
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

		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveUp), this, &ALyssa::MoveUp).bExecuteWhenPaused = true;
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveDown), this, &ALyssa::MoveDown).bExecuteWhenPaused = true;
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveLeft), this, &ALyssa::MoveLeft).bExecuteWhenPaused = true;
		playerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveRight), this, &ALyssa::MoveRight).bExecuteWhenPaused = true;
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
	ALevelGameMode* currentLevelGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (currentLevelGameMode)
	{
		//check for CurrentGameMode begin completion, wait if not completed

		//UE_LOG(LogTemp, Log, TEXT("before waiting for begin function completion"));
		WaitForLoadCompletionAndAssignKeys(currentLevelGameMode, PlayerInputComponent, 0.1f);
		//UE_LOG(LogTemp, Log, TEXT("after waiting for begin function completion"));
	}
}

#pragma region Menu
//_______________
void ALyssa::MoveMenuUp()
{
	if (DEBUG) UE_LOG(LogTemp, Log, TEXT("MoveMenuUp"));
	OnUpDelegate.Broadcast();
}

void ALyssa::MoveMenuDown()
{
	if (DEBUG) UE_LOG(LogTemp, Log, TEXT("MoveMenuDown"));
	OnDownDelegate.Broadcast();
}

void ALyssa::MoveMenuLeft()
{
	if (DEBUG) UE_LOG(LogTemp, Log, TEXT("MoveMenuLeft"));
	OnLeftDelegate.Broadcast();
}

void ALyssa::MoveMenuRight()
{
	if (DEBUG) UE_LOG(LogTemp, Log, TEXT("MoveMenuRight"));
	OnRightDelegate.Broadcast();
}

//_______________
#pragma endregion


#pragma region Lyssa
//_______________
void ALyssa::MoveUp(float value)
{
	//if (DEBUG) UE_LOG(LogTemp, Log, TEXT("topKeyValue = %f"), topKeyValue);

	if (Controller != NULL && value != 0)
	{
		//if (value * value > 0.5f * 0.5f)
		//	OnUpDelegate.Broadcast();
		if (value > 0.5f)
		{
			OnUpDelegate.Broadcast();
		}
		else if (value < -0.5f)
		{
			OnDownDelegate.Broadcast();
		}

		// find out which way is up
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get direction vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, value);

		IsMoving = true;
		Timer = 0;

		// Set Character's rotation
		topKeyValue = value;
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("topKeyValue = %f"), topKeyValue);
	}
}

void ALyssa::MoveDown(float value)
{
	if (Controller != NULL && value != 0)
	{
		//if (value * value > 0.5f * 0.5f)
		//	OnDownDelegate.Broadcast();
		if (value > 0.5f)
		{
			OnDownDelegate.Broadcast();
		}
		else if (value < -0.5f)
		{
			OnUpDelegate.Broadcast();
		}

		// find out which way is up
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get direction vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, -value);

		IsMoving = true;
		Timer = 0;

		// Set Character's rotation
		downKeyValue = value;
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("downKeyValue = %f"), downKeyValue);
	}
}

void ALyssa::MoveRight(float value)
{
	if (Controller != NULL && value != 0)
	{
		//if (value * value > 0.5f * 0.5f)
		//	OnRightDelegate.Broadcast();
		if (value > 0.5f)
		{
			OnRightDelegate.Broadcast();
		}
		else if (value < -0.5f)
		{
			OnLeftDelegate.Broadcast();
		}

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, value);

		IsMoving = true;
		Timer = 0;

		// Set Character's rotation
		rightKeyValue = value;
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("rightKeyValue = %f"), rightKeyValue);
	}
}

void ALyssa::MoveLeft(float value)
{
	if (Controller != NULL && value != 0)
	{
		//if (value * value > 0.5f * 0.5f)
		//	OnLeftDelegate.Broadcast();
		if (value > 0.5f)
		{
			OnLeftDelegate.Broadcast();
		}
		else if (value < -0.5f)
		{
			OnRightDelegate.Broadcast();
		}

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, -value);

		IsMoving = true;
		Timer = 0;

		// Set Character's rotation
		leftKeyValue = value;
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("leftKeyValue = %f"), leftKeyValue);
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

	this->LyssaSKMesh->SetRelativeRotation(FRotator(0, rotationAngle, 0));
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
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("ftopKeyValue = %f"), this->Fylgja->fTopKeyValue);
	}
}

void ALyssa::MoveFDown(float value)
{
	if (value != 0)
	{
		// Set Character's rotation
		this->Fylgja->fDownKeyValue = value;
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("fdownKeyValue = %f"), this->Fylgja->fDownKeyValue);
	}
}

void ALyssa::MoveFRight(float value)
{
	if (value != 0)
	{
		// Set Character's rotation
		this->Fylgja->fRightKeyValue = value;
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("frightKeyValue = %f"), this->Fylgja->fRightKeyValue);
	}
}

void ALyssa::MoveFLeft(float value)
{
	if (value != 0)
	{
		// Set Character's rotation
		this->Fylgja->fLeftKeyValue = value;
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("fleftKeyValue = %f"), this->Fylgja->fLeftKeyValue);
	}
}
//_______________
#pragma endregion


void ALyssa::ActionAccept()
{
	if (DEBUG) UE_LOG(LogTemp, Log, TEXT("ActionAccept"));
	OnAcceptDelegate.Broadcast();
}

void ALyssa::ActionReturn()
{
	if (DEBUG) UE_LOG(LogTemp, Log, TEXT("ActionReturn"));
	OnReturnDelegate.Broadcast();
}

void ALyssa::PauseGame()
{
	// === GameMode ===
	ALevelGameMode* CurrentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (CurrentGameMode && CurrentGameMode->IsPauseAllowed && CurrentGameMode->GetCurrentState() != ELevelPlayState::EGameOver
		&& CurrentGameMode->GetCurrentState() != ELevelPlayState::ELevelCompleted)//IsPauseAllowed
	{
		if (CurrentGameMode->GetCurrentState() != ELevelPlayState::EPause)
		{
			if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("Pause called"));
			CurrentGameMode->SetCurrentState(ELevelPlayState::EPause);
		}
		else
		{
			if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("Unpaused"));
			CurrentGameMode->SetCurrentState(ELevelPlayState::EPlaying);
		}

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
	this->GetOverlappingActors(collectedActors);

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

			if (castedLife && Life != MaxLife)
			{
				UpdateLife(+castedLife->GetLifeAmount());
				castedPickup->SetActive(false); //deactivate the life pickup
			}
			else if (castedScore)
			{
				UpdateScorePickupAmount(+castedScore->GetScoreAmount());
				castedPickup->SetActive(false); //deactivate the score pickup
			}
			else if (castedKey)
			{
				AddKeyPickup(castedKey->GetKeyNature());
				castedPickup->SetActive(false); //deactivate the key pickup
			}
			//else
			//{
			//	castedPickup->SetActive(false); //deactivate the basic pickup
			//}
		}
	}
}
//____________________________________________________________________________________
#pragma endregion
