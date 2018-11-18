// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelPortal.h"
#include "Characters/Lyssa/Lyssa.h"

// Sets default values
ALevelPortal::ALevelPortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	RootComponent = (USceneComponent*)PortalMesh;

	PortalInterior = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalInterior"));
	PortalInterior->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ALevelPortal::BeginPlay()
{
	Super::BeginPlay();

}

void ALevelPortal::SwapLevel()
{
	UWorld* currentWorld = GetWorld();

	FString CurrentLevel = currentWorld->GetMapName();
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void ALevelPortal::HandleOverlap()
{
	//get overlaping actors and store them in an array
	TArray<AActor*> collectedActors;
	PortalInterior->GetOverlappingActors(collectedActors);

	for (size_t i = 0; i < collectedActors.Num(); ++i)
	{
		AActor* currentActor = collectedActors[i];
		ALyssa* currentLyssa = Cast<ALyssa>(currentActor);
		if (currentLyssa)
		{
			//go to level
			SwapLevel();
		}
	}
}

// Called every frame
void ALevelPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleOverlap();
}

