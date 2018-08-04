// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelGameController.h"


// Sets default values
ALevelGameController::ALevelGameController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelGameController::BeginPlay()
{
	Super::BeginPlay();

	if (FinishArea == nullptr)
		UE_LOG(LogTemp, Error, TEXT("No FinishArea was added to LevelController"));

	Lyssa = (ALyssa*)UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	IsLevelCompleted = false;
	IsGameOver = false;

	Rabbits.Empty(Rabbits.Num());
	for (TActorIterator<ARabbit> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ARabbit *rabbit = *ActorItr;
		Rabbits.Add(rabbit);
	}
}

void ALevelGameController::HandleFylgjaReflect()
{
	for (size_t i = 0; i < Rabbits.Num(); i++)
	{
		ARabbit* rabbit = Rabbits[i];
		float sqrDist = FVector::DistSquared(rabbit->GetActorLocation(), Lyssa->GetActorLocation());

		if (sqrDist < 200.0f * CollisionDistThreshold * CollisionDistThreshold)
		{
			//UE_LOG(LogTemp, Log, TEXT("if1"));
			for (size_t j = 0; j < rabbit->Shots.Num(); j++)
			{
				AShot* shot = rabbit->Shots[j];

				FRotator rotF = Lyssa->Fylgja->GetActorRotation();
				FVector fylgjaDir = rotF.Vector().GetSafeNormal();

				//shot->GetActorLocation() + 300.0f*fylgjaDir
				float sqrDistSF = FVector::DistSquared(shot->GetActorLocation(), Lyssa->Fylgja->GetActorLocation());
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

void ALevelGameController::HandleProjectileDamage()
{
	for (size_t i = 0; i < Rabbits.Num(); i++)
	{
		ARabbit* rabbit = Rabbits[i];
		float sqrDist = FVector::DistSquared(rabbit->GetActorLocation(), Lyssa->GetActorLocation());
		if (sqrDist < CollisionDistThreshold * CollisionDistThreshold)
		{
			FString TheFloatStr = FString::SanitizeFloat(Lyssa->Life);
			TheFloatStr = TEXT("Rabit hurts Lyssa | life = ") + TheFloatStr;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);

			Lyssa->Life -= 1.0f;
		}

		for (size_t j = 0; j < rabbit->Shots.Num(); j++)
		{
			AShot* shot = rabbit->Shots[j];

			float sqrDistSL = FVector::DistSquared(shot->GetActorLocation(), Lyssa->GetActorLocation());
			if (!shot->ShouldBeDestroy && sqrDistSL < CollisionDistThreshold * CollisionDistThreshold)
			{
				Lyssa->Life -= 10.0f;

				FString TheFloatStr = FString::SanitizeFloat(Lyssa->Life);
				TheFloatStr = TEXT("Projectile hurts Lyssa | life = ") + TheFloatStr;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);

				shot->ShouldBeDestroy = true;
			}

			float sqrDistSR = FVector::DistSquared(shot->GetActorLocation(), rabbit->GetActorLocation());
			if (shot->CanKillFoe && !shot->ShouldBeDestroy
				&& sqrDistSR < 2.0f * CollisionDistThreshold * CollisionDistThreshold)
			{
				rabbit->Life -= 10.0f;

				FString TheFloatStr = FString::SanitizeFloat(rabbit->Life);
				TheFloatStr = TEXT("Projectile hurts rabbit | life = ") + TheFloatStr;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, *TheFloatStr);

				shot->ShouldBeDestroy = true;
			}
		}
	}

}

void ALevelGameController::CheckForLevelCompleted()
{
	float DistanceToEnd = FVector::DistSquared(FinishArea->GetActorLocation(), Lyssa->GetActorLocation());
	if (!IsLevelCompleted && DistanceToEnd < FinishArea->FARadius * FinishArea->FARadius)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lyssa is in finish area!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Lyssa is in finish area!"));

		IsLevelCompleted = true;
		AMainGameMode* gameMode = (AMainGameMode*)GetWorld()->GetAuthGameMode();
		if (gameMode)
			gameMode->ShowEndingWidget();
	}
}

void ALevelGameController::CheckForDeath()
{
	if (Lyssa->Life < 0.0f)
	{
		if (!IsGameOver)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lyssa died | game over"));
			IsGameOver = true;
		}
		return;
	}

	TArray<int32> indexes;
	indexes.Reset(0);
	for (size_t i = 0; i < Rabbits.Num(); i++)
	{
		ARabbit* rabbit = Rabbits[i];
		if (rabbit->ShouldBeDestroyed)
			indexes.Add(i);
	}
	for (size_t i = 0; i < indexes.Num(); i++)
	{
		Rabbits[indexes[i]]->Destroy();
		Rabbits.RemoveAt(indexes[i]);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Rabbit killed"));
	}
}

// Called every frame
void ALevelGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DamageRateTimer -= DeltaTime;
	if (DamageRateTimer < DamageRate)
	{
		HandleProjectileDamage();
		DamageRateTimer = DamageRate;
	}

	HandleFylgjaReflect();

	CheckForDeath();

	CheckForLevelCompleted();
}

