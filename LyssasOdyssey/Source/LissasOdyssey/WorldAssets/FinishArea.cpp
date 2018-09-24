#include "FinishArea.h"
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

}

// Called every frame
void AFinishArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), FARadius, 32, FColor(255, 0, 0));
}

