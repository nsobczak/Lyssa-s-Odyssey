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

	//UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ALevelGameController::CheckForLevelCompleted() {

	float DistanceToEnd = FVector::DistSquared(FinishArea->GetActorLocation(), Lyssa->GetActorLocation());
	if (!IsLevelCompleted && DistanceToEnd < FinishArea->FARadius * FinishArea->FARadius)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lyssa is in finish area!"));
		IsLevelCompleted = true;
	}
}

// Called every frame
void ALevelGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForLevelCompleted();
}

