#include "Fylgja.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"  
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h" 
#include "GameModes/LevelGameMode.h"
#include "Utils/GameConstants.h"


// Sets default values
AFylgja::AFylgja(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	FylgjaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FylgjaMesh"));
	RootComponent = (USceneComponent*)FylgjaMesh;

	////create the skeletal mesh components
	//FylgjaTop = CreateDefaultSubobject<USkeletalMesh>(TEXT("FylgjaTop"));
	//FylgjaBottom = CreateDefaultSubobject<USkeletalMesh>(TEXT("FylgjaBottom"));
}

// Called when the game starts or when spawned
void AFylgja::BeginPlay()
{
	Super::BeginPlay();

	FylgjaMesh->SetVisibility(false, false); //fylgja collider
}

//_____________________________________________________________________________________________


void AFylgja::ShowFylfja()
{
	IsVisible = true;
	FylgjaMesh->SetVisibility(true, true);
	FylgjaMesh->SetVisibility(false, false); //fylgja collider
}

void AFylgja::HideFylfja()
{
	IsVisible = false;
	FylgjaMesh->SetVisibility(false, true);
}

#pragma region position
//____________________________________________________________________________________

void AFylgja::FollowMousePosition()
{
	//AFylgja *Character = Cast<AFylgja>(GetPawn());
	APlayerController* playercontroller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Get mouse position on screen
	float xMouse, yMouse;
	playercontroller->GetMousePosition(xMouse, yMouse);

	// Get Character position on screen
	FVector CharLoc = this->GetActorLocation();
	FVector2D CharInScreen;
	playercontroller->ProjectWorldLocationToScreen(CharLoc, CharInScreen);

	// Get mouse position relative to the Character.
	FVector2D Result;
	Result.X = -(yMouse - CharInScreen.Y);
	Result.Y = xMouse - CharInScreen.X;

	// Get angle rotation and rotation Character
	float angle = FMath::RadiansToDegrees(FMath::Acos(Result.X / Result.Size()));

	if (Result.Y < 0)
		angle = 360 - angle;

	FRotator rot(0, angle, 0);

	this->SetActorRotation(rot);
}


void AFylgja::UpdateFRotation()
{
	//moving average
	FStructStickInputs val = FStructStickInputs(fTopKeyValue, fRightKeyValue, fDownKeyValue, fLeftKeyValue);

	if (this->FRotationValues.Num() < UGameConstants::FROTATION_VALUE_COUNT)
	{
		for (size_t i = 0; i < UGameConstants::FROTATION_VALUE_COUNT - this->FRotationValues.Num(); ++i)
		{
			this->FRotationValues.Insert(val, 0);
		}
	}
	else
	{
		this->FRotationValues.Insert(val, 0);
		this->FRotationValues.RemoveAt(UGameConstants::FROTATION_VALUE_COUNT, 1, true);
	}
	//UE_LOG(LogTemp, Log, TEXT("this->FRotationValues.Num() = %i"), this->FRotationValues.Num());

	FStructStickInputs buffer = FStructStickInputs(0, 0, 0, 0);
	for (size_t i = 0; i < this->FRotationValues.Num(); ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			buffer.Inputs[j] += this->FRotationValues[i].Inputs[j];
		}
	}
	for (size_t j = 0; j < 4; ++j)
	{
		val.Inputs[j] += buffer.Inputs[j] / this->FRotationValues.Num();
	}

	fTopKeyValue = val.Inputs[0];
	fRightKeyValue = val.Inputs[1];
	fDownKeyValue = val.Inputs[2];
	fLeftKeyValue = val.Inputs[3];


	//1 value
	if (fLeftKeyValue != 0 && fTopKeyValue == 0 && fRightKeyValue == 0 && fDownKeyValue == 0)
	{
		if (fLeftKeyValue > 0)
			fRotationAngle = 90.0f;
		else
			fRotationAngle = -90.0f;
	}
	else if (fLeftKeyValue == 0 && fTopKeyValue != 0 && fRightKeyValue == 0 && fDownKeyValue == 0)
	{
		if (fTopKeyValue > 0)
			fRotationAngle = 180.0f;
		else
			fRotationAngle = 0.0f;
	}
	else if (fLeftKeyValue == 0 && fTopKeyValue == 0 && fRightKeyValue != 0 && fDownKeyValue == 0)
	{
		if (fRightKeyValue > 0)
			fRotationAngle = 90.0f;
		else
			fRotationAngle = -90.0f;
	}
	else if (fLeftKeyValue == 0 && fTopKeyValue == 0 && fRightKeyValue == 0 && fDownKeyValue != 0)
	{
		if (fDownKeyValue > 0)
			fRotationAngle = 180.0f;
		else
			fRotationAngle = 0.0f;
	}

	//2 values
	if (fLeftKeyValue == 0 && fTopKeyValue != 0 && fRightKeyValue != 0 && fDownKeyValue == 0)
	{
		fRotationAngle = atan2f(fRightKeyValue, -fTopKeyValue) * 180 / PI;
	}
	else if (fLeftKeyValue == 0 && fTopKeyValue == 0 && fRightKeyValue != 0 && fDownKeyValue != 0)
	{
		fRotationAngle = 90.0f + atan2f(-fDownKeyValue, fRightKeyValue) * 180 / PI;
	}
	else if (fLeftKeyValue != 0 && fTopKeyValue == 0 && fRightKeyValue == 0 && fDownKeyValue != 0)
	{
		fRotationAngle = 180.0f + atan2f(fLeftKeyValue, -fDownKeyValue) * 180 / PI;
	}
	else if (fLeftKeyValue != 0 && fTopKeyValue != 0 && fRightKeyValue == 0 && fDownKeyValue == 0)
	{
		fRotationAngle = 270.0f + atan2f(-fTopKeyValue, fLeftKeyValue) * 180 / PI;
	}

	//UE_LOG(LogTemp, Log, TEXT("fRotationAngle = %f"), fRotationAngle);
	this->SetActorRotation(FRotator(0, fRotationAngle, 0));


	fLeftKeyValue = 0; fTopKeyValue = 0; fRightKeyValue = 0; fDownKeyValue = 0;
}

//____________________________________________________________________________________
#pragma endregion

// Called every frame
void AFylgja::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// === FollowMousePosition ===
	ALevelGameMode* CurrentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (CurrentGameMode)
	{
		if (CurrentGameMode->UseGamePad)
		{
			UpdateFRotation();
		}
		else
		{
			FollowMousePosition();
		}
	}
}
