#include "FinishArea.h"
#include "GameModes/LevelGameMode.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"


// Sets default values
AFinishArea::AFinishArea(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFinishArea::BeginPlay()
{
	Super::BeginPlay();

	IsLevelFinished = false;
}

// Called every frame
void AFinishArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsLevelFinished)
	{
		//get overlaping actors and store them in an array
		TArray<AActor*> overlappingActors;
		BaseMesh->GetOverlappingActors(overlappingActors);
		for (size_t i = 0; i < overlappingActors.Num(); ++i)
		{
			ALyssa* currentLyssa = Cast<ALyssa>(overlappingActors[i]);
			ALevelGameMode* currentGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
			if (currentLyssa && currentGameMode)
			{
				currentGameMode->SetCurrentState(ELevelPlayState::ELevelCompleted);
				IsLevelFinished = true;
			}
		}
	}

}

