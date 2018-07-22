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

	Rabbits.Empty(Rabbits.Num());
	for (TActorIterator<ARabbit> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ARabbit *rabbit = *ActorItr;
		Rabbits.Add(rabbit);
	}
}

void ALevelGameController::HandleProjectileDamage() {

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

			float sqrDist = FVector::DistSquared(shot->GetActorLocation(), Lyssa->GetActorLocation());
			if (!shot->ShouldBeDestroy && sqrDist < CollisionDistThreshold * CollisionDistThreshold)
			{
				Lyssa->Life -= 1.0f;

				FString TheFloatStr = FString::SanitizeFloat(Lyssa->Life);
				TheFloatStr = TEXT("Projectile hurts Lyssa | life = ") + TheFloatStr;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);
				
				shot->ShouldBeDestroy = true;
			}

			sqrDist = FVector::DistSquared(shot->GetActorLocation(), rabbit->GetActorLocation());
			if (!shot->ShouldBeDestroy && sqrDist < CollisionDistThreshold * CollisionDistThreshold)
			{
				rabbit->Life -= 3.0f;

				FString TheFloatStr = FString::SanitizeFloat(rabbit->Life);
				TheFloatStr = TEXT("Projectile hurts rabbit | life = ") + TheFloatStr;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);

				shot->ShouldBeDestroy = true;
			}
		}

	}
}

void ALevelGameController::CheckForLevelCompleted() {

	float DistanceToEnd = FVector::DistSquared(FinishArea->GetActorLocation(), Lyssa->GetActorLocation());
	if (!IsLevelCompleted && DistanceToEnd < FinishArea->FARadius * FinishArea->FARadius)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lyssa is in finish area!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Lyssa is in finish area!"));

		IsLevelCompleted = true;
	}
}

void ALevelGameController::CheckForDeath() {
	if (Lyssa->Life < 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lyssa died | game over"));
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

	CheckForDeath();

	CheckForLevelCompleted();
}

