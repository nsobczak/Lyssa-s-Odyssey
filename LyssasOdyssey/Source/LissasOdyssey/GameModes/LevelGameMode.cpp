// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGameMode.h"
#include "GameModes/MainGameMode.h"
#include "Characters/CharacterActors/Shot.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" //to be able to access the character
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ALevelGameMode::ALevelGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ALyssa::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("/Game/Characters/Lyssa/BPLyssa"));
	if (PlayerPawnObject.Succeeded())
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}

	//HUDClass = AFPSHUD::StaticClass();
}


// Called when the game starts or when spawned
void ALevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(ELevelPlayState::EPlaying);

	//hud
	if (nullptr != HUDClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (nullptr != CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	LevelTimer = 0;
}


//_____________________________________________________________________________________________

FText ALevelGameMode::GetTimerForHud()
{
	//timer is in second
	int seconds = (int)LevelTimer;
	int minutes = seconds / 60;
	int hours = minutes / 60;

	seconds %= 60;
	minutes %= 60;

	FString fSeconds = FString::FromInt(seconds) + FString("\"");
	FString fMinutes = FString::FromInt(minutes) + FString("\'");
	FString fHours = FString::FromInt(hours) + FString(":");

	FString textToShow = hours > 0 ? fHours + fMinutes + fSeconds : fMinutes + fSeconds;
	return FText::FromString(textToShow);
}

void ALevelGameMode::CheckForLevelCompleted()
{
	float DistanceToEnd = FVector::DistSquared(FinishArea->GetActorLocation(), Lyssa->GetActorLocation());
	if (DistanceToEnd < FinishArea->FARadius * FinishArea->FARadius)
	{
		SetCurrentState(ELevelPlayState::ELevelCompleted);
	}
}

void ALevelGameMode::CheckForDeath()
{
	if (Lyssa->GetCurrentLife() < 0.0f)
	{
		SetCurrentState(ELevelPlayState::EGameOver);
	}
}

//_____________________________________________________________________________________________

// Called every frame
void ALevelGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LevelTimer += DeltaTime;

	DamageRateTimer -= DeltaTime;
	if (DamageRateTimer < DamageRate)
	{
		//HandleProjectileDamage();
		DamageRateTimer = DamageRate;
	}

	if (Currentstate == ELevelPlayState::EPlaying)
	{
		CheckForLevelCompleted();
		CheckForDeath();
	}
}


#pragma region PlayStates

ELevelPlayState ALevelGameMode::GetCurrentState() const
{
	return Currentstate;
}

void ALevelGameMode::SetCurrentState(ELevelPlayState newState)
{
	Currentstate = newState;
	HandleNewState(Currentstate);
}

void ALevelGameMode::HandleNewState(ELevelPlayState newState)
{
	switch (newState)
	{
	case ELevelPlayState::EPlaying:
	{
		// find all finishArea in scene
		TArray<AActor*> foundFinnishAreas;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFinishArea::StaticClass(), foundFinnishAreas);
		if (foundFinnishAreas.Num() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("No FinishArea was added to LevelController"));
		}
		FinishArea = Cast<AFinishArea>(foundFinnishAreas[0]);

		//Lyssa
		Lyssa = Cast<ALyssa>(UGameplayStatics::GetPlayerPawn(this, 0));

		break;
	}

	case ELevelPlayState::EGameOver:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lyssa died | game over"));

		AMainGameMode* gameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			//block player input
			APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
			if (playerController)
			{
				playerController->SetCinematicMode(true, false, false, true, true);
			}
			gameMode->ShowGameOverWidget();
		}

		break;
	}

	case ELevelPlayState::ELevelCompleted:
	{
		UE_LOG(LogTemp, Warning, TEXT("Lyssa is in finish area!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Lyssa is in finish area!"));

		AMainGameMode* gameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
		if (gameMode)
		{
			//block player input
			APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
			if (playerController)
			{
				playerController->SetCinematicMode(true, false, false, true, true);
			}
			gameMode->ShowEndingWidget();
		}

		break;
	}

	case ELevelPlayState::EUnknown:
		//do nothing
		break;

	default:
		//do nothing
		break;
	}
}

#pragma endregion

