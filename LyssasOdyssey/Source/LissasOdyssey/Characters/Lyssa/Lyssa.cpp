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

	ScorePickupAmount = 0; //TODO: check how many score pickup we already have picked up in current level
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

	////keep same z value - used only when colliding
	//FVector loc = GetActorLocation();
	//if (loc.Z != initialPosZValue)
	//{
	//	//UE_LOG(LogTemp, Log, TEXT("keep same z value for lyssa position"));
	//	SetActorLocation(FVector(loc.X, loc.Y, initialPosZValue));
	//}

	CollectPickups();
}

AFylgja* ALyssa::GetFylgja() const
{
	return Fylgja;
}

#pragma region Input
//____________________________________________________________________________________

// Called to bind functionality to input
void ALyssa::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// === GameMode ===
	ALevelGameMode* CurrentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (CurrentGameMode)
	{
		//TODO: debind all

		//TArray<FKey> keys = CurrentGameMode->KeyListKeyboard;
		//UE_LOG(LogTemp, Log, TEXT("keys: %s %s %s %s %s %s"), *(keys[0].ToString()), *(keys[1].ToString()),
		//	*(keys[2].ToString()), *(keys[3].ToString()), *(keys[4].ToString()), *(keys[5].ToString()));

		////// set up gameplay key bindings
		////PlayerInputComponent->BindAxis("MoveUp", this, &ALyssa::MoveUp);
		////PlayerInputComponent->BindAxis("MoveRight", this, &ALyssa::MoveRight);
		//PlayerInputComponent->BindAxisKey(keys[0], this, &ALyssa::MoveUp);
		//PlayerInputComponent->BindAxisKey(keys[1], this, &ALyssa::MoveDown);
		//PlayerInputComponent->BindAxisKey(keys[2], this, &ALyssa::MoveLeft);
		//PlayerInputComponent->BindAxisKey(keys[3], this, &ALyssa::MoveRight);
		//PlayerInputComponent->BindKey(keys[4], EInputEvent::IE_Released, this, &ALyssa::ActionAccept).bExecuteWhenPaused = true;
		//PlayerInputComponent->BindKey(keys[5], EInputEvent::IE_Released, this, &ALyssa::ActionReturn).bExecuteWhenPaused = true;
		//PlayerInputComponent->BindKey(keys[6], EInputEvent::IE_Released, this, &ALyssa::PauseGame).bExecuteWhenPaused = true;

		//keys = CurrentGameMode->KeyListGamepad;
		//UE_LOG(LogTemp, Log, TEXT("keys: %s %s %s %s %s %s"), *(keys[0].ToString()), *(keys[1].ToString()),
		//	*(keys[2].ToString()), *(keys[3].ToString()), *(keys[4].ToString()), *(keys[5].ToString()));
		//PlayerInputComponent->BindAxisKey(keys[0], this, &ALyssa::MoveUp);
		//PlayerInputComponent->BindAxisKey(keys[1], this, &ALyssa::MoveDown);
		//PlayerInputComponent->BindAxisKey(keys[2], this, &ALyssa::MoveLeft);
		//PlayerInputComponent->BindAxisKey(keys[3], this, &ALyssa::MoveRight);
		//PlayerInputComponent->BindKey(keys[4], EInputEvent::IE_Released, this, &ALyssa::ActionAccept).bExecuteWhenPaused = true;
		//PlayerInputComponent->BindKey(keys[5], EInputEvent::IE_Released, this, &ALyssa::ActionReturn).bExecuteWhenPaused = true;
		//PlayerInputComponent->BindKey(keys[6], EInputEvent::IE_Released, this, &ALyssa::PauseGame).bExecuteWhenPaused = true;

		TMap<TEnumAsByte<PlayerActionLabel>, FKey>TMapKeys = CurrentGameMode->TMapKeyboardKeys;
		UE_LOG(LogTemp, Log, TEXT("keyboard keys: %s %s %s %s %s %s %s"), *(TMapKeys.FindRef(PlayerActionLabel::MoveUp).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveDown).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::MoveLeft).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveRight).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::ACross).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::ATriangle).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::AStart).ToString()));
		// set up gameplay key bindings
		//PlayerInputComponent->BindAxis("MoveUp", this, &ALyssa::MoveUp);
		//PlayerInputComponent->BindAxis("MoveRight", this, &ALyssa::MoveRight);
		PlayerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveUp), this, &ALyssa::MoveUp);
		PlayerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveDown), this, &ALyssa::MoveDown);
		PlayerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveLeft), this, &ALyssa::MoveLeft);
		PlayerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveRight), this, &ALyssa::MoveRight);
		PlayerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ACross), EInputEvent::IE_Released, this, &ALyssa::ActionAccept).bExecuteWhenPaused = true;
		PlayerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ATriangle), EInputEvent::IE_Released, this, &ALyssa::ActionReturn).bExecuteWhenPaused = true;
		PlayerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::AStart), EInputEvent::IE_Released, this, &ALyssa::PauseGame).bExecuteWhenPaused = true;

		TMapKeys = CurrentGameMode->TMapGamepadKeys;
		UE_LOG(LogTemp, Log, TEXT("keyboard keys: %s %s %s %s %s %s %s"), *(TMapKeys.FindRef(PlayerActionLabel::MoveUp).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveDown).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::MoveLeft).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::MoveRight).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::ACross).ToString()),
			*(TMapKeys.FindRef(PlayerActionLabel::ATriangle).ToString()), *(TMapKeys.FindRef(PlayerActionLabel::AStart).ToString()));
		PlayerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveUp), this, &ALyssa::MoveUp);
		PlayerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveDown), this, &ALyssa::MoveDown);
		PlayerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveLeft), this, &ALyssa::MoveLeft);
		PlayerInputComponent->BindAxisKey(TMapKeys.FindRef(PlayerActionLabel::MoveRight), this, &ALyssa::MoveRight);
		PlayerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ACross), EInputEvent::IE_Released, this, &ALyssa::ActionAccept).bExecuteWhenPaused = true;
		PlayerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::ATriangle), EInputEvent::IE_Released, this, &ALyssa::ActionReturn).bExecuteWhenPaused = true;
		PlayerInputComponent->BindKey(TMapKeys.FindRef(PlayerActionLabel::AStart), EInputEvent::IE_Released, this, &ALyssa::PauseGame).bExecuteWhenPaused = true;
	}
}

void ALyssa::MoveUp(float value)
{
	if (Controller != NULL && value != 0)
	{
		// find out which way is up
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get direction vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, value);

		// Set Character's rotation
		this->GetMesh()->SetRelativeRotation(FRotator(0, 0, 0));
	}
}

void ALyssa::MoveDown(float value)
{
	if (Controller != NULL && value != 0)
	{
		// find out which way is up
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get direction vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, -value);

		// Set Character's rotation
		this->GetMesh()->SetRelativeRotation(FRotator(0, 180.0f, 0));
	}
}

void ALyssa::MoveRight(float value)
{
	if (Controller != NULL && value != 0)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, value);

		// Set Character's rotation
		this->GetMesh()->SetRelativeRotation(FRotator(0, 90.0f, 0));
	}
}

void ALyssa::MoveLeft(float value)
{
	if (Controller != NULL && value != 0)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, -value);

		// Set Character's rotation
		this->GetMesh()->SetRelativeRotation(FRotator(0, 270.0f, 0));
	}
}

void ALyssa::ActionAccept()
{
	UE_LOG(LogTemp, Log, TEXT("ActionAccept"));
}

void ALyssa::ActionReturn()
{
	UE_LOG(LogTemp, Log, TEXT("ActionReturn"));
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


#pragma region Pickup
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

			//check to see if the pickup is life
			APickupLife* const castedLife = Cast<APickupLife>(castedPickup);
			APickupScore* const castedScore = Cast<APickupScore>(castedPickup);

			if (castedLife)
				UpdateLife(+castedLife->GetLifeAmount());
			else if (castedScore)
				UpdateScorePickupAmount(+castedScore->GetScoreAmount());

			//deactivate the pickup
			castedPickup->SetActive(false);
		}
	}
}

//____________________________________________________________________________________
#pragma endregion
