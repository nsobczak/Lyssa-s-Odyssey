// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGameMode.h"
#include "GameModes/MainGameMode.h"
#include "Characters/CharacterActors/Shot.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" //to be able to access the character
#include "Kismet/KismetMathLibrary.h"

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
}

//_____________________________________________________________________________________________

void ALevelGameMode::HandleFylgjaReflect()
{
	for (size_t i = 0; i < Foes.Num(); i++)
	{
		AFoe* foe = Foes[i];
		float sqrDist = FVector::DistSquared(foe->GetActorLocation(), Lyssa->GetActorLocation());

		if (sqrDist < 200.0f * CollisionDistThreshold * CollisionDistThreshold)
		{
			//UE_LOG(LogTemp, Log, TEXT("if1"));
			for (size_t j = 0; j < foe->Shots.Num(); j++)
			{
				AShot* shot = foe->Shots[j];

				FRotator rotF = Lyssa->GetFylgja()->GetActorRotation();
				FVector fylgjaDir = rotF.Vector().GetSafeNormal();

				//shot->GetActorLocation() + 300.0f*fylgjaDir
				float sqrDistSF = FVector::DistSquared(shot->GetActorLocation(), Lyssa->GetFylgja()->GetActorLocation());
				//UE_LOG(LogTemp, Log, TEXT("dist shot actor = %f"), sqrDistSF);

				if (sqrDistSF < 1.5f *CollisionDistThreshold * CollisionDistThreshold)
				{
					shot->targetDirection = fylgjaDir;
					shot->CanKillFoe = true;
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Changed target direction"));
				}
			}
		}
	}

}

void ALevelGameMode::HandleProjectileDamage()
{
	for (size_t i = 0; i < Foes.Num(); i++)
	{
		AFoe* foe = Foes[i];
		float sqrDist = FVector::DistSquared(foe->GetActorLocation(), Lyssa->GetActorLocation());
		if (sqrDist < CollisionDistThreshold * CollisionDistThreshold)
		{
			FString TheFloatStr = FString::SanitizeFloat(Lyssa->GetCurrentLife());
			TheFloatStr = TEXT("Rabit hurts Lyssa | life = ") + TheFloatStr;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);

			Lyssa->UpdateLife(-1.0f);
		}

		for (size_t j = 0; j < foe->Shots.Num(); j++)
		{
			AShot* shot = foe->Shots[j];

			float sqrDistSL = FVector::DistSquared(shot->GetActorLocation(), Lyssa->GetActorLocation());
			if (!shot->ShouldBeDestroy && sqrDistSL < CollisionDistThreshold * CollisionDistThreshold)
			{
				Lyssa->UpdateLife(-10.0f);

				FString TheFloatStr = FString::SanitizeFloat(Lyssa->GetCurrentLife());
				TheFloatStr = TEXT("Projectile hurts Lyssa | life = ") + TheFloatStr;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);

				shot->ShouldBeDestroy = true;
			}

			float sqrDistSR = FVector::DistSquared(shot->GetActorLocation(), foe->GetActorLocation());
			if (shot->CanKillFoe && !shot->ShouldBeDestroy
				&& sqrDistSR < 2.0f * CollisionDistThreshold * CollisionDistThreshold)
			{
				foe->UpdateLife(-10.0f);

				FString TheFloatStr = FString::SanitizeFloat(foe->GetCurrentLife());
				TheFloatStr = TEXT("Projectile hurts foe | life = ") + TheFloatStr;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, *TheFloatStr);

				shot->ShouldBeDestroy = true;
			}
		}
	}

}

//_____________________________________________________________________________________________

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
	else
	{
		//handles foes death

		TArray<int32> indexes;
		indexes.Reset(0);
		for (size_t i = 0; i < Foes.Num(); i++)
		{
			AFoe* foe = Foes[i];
			if (foe->ShouldBeDestroyed)
				indexes.Add(i);
		}
		for (size_t i = 0; i < indexes.Num(); i++)
		{
			Foes[indexes[i]]->CustomDestroy();
			Foes.RemoveAt(indexes[i]);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Foe killed"));
		}
	}
}

//_____________________________________________________________________________________________

// Called every frame
void ALevelGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DamageRateTimer -= DeltaTime;
	if (DamageRateTimer < DamageRate)
	{
		HandleProjectileDamage();
		DamageRateTimer = DamageRate;
	}

	if (Currentstate == ELevelPlayState::EPlaying)
	{
		HandleFylgjaReflect();

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

		//foes
		Foes.Empty(Foes.Num());
		for (TActorIterator<AFoe> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AFoe *foe = *ActorItr;
			Foes.Add(foe);
		}

		break;
	}

	case ELevelPlayState::EGameOver:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lyssa died | game over"));

		//block player input
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		if (playerController)
		{
			playerController->SetCinematicMode(true, false, false, true, true);
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

