// Fill out your copyright notice in the Description page of Project Settings.

#include "HindranceRock.h"
//#include "DestructibleMesh.h"
#include "Components/StaticMeshComponent.h"
//#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/CharacterActors/Shot.h"

// Sets default values
AHindranceRock::AHindranceRock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the mesh component
	HindranceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HindranceMesh"));
	RootComponent = (USceneComponent*)HindranceMesh;

	//HindranceDM = CreateDefaultSubobject<UDestructibleMesh>(TEXT("HindranceDestructibleMesh"));
	//RootComponent = (USceneComponent*)HindranceDM;
}

// Called when the game starts or when spawned
void AHindranceRock::BeginPlay()
{
	Super::BeginPlay();

	HindranceMesh->SetVisibility(false);
}

void AHindranceRock::HandleOverlap_Implementation()
{
	//get overlaping actors and store them in an array
	TArray<AActor*> collectedActors;
	HindranceMesh->GetOverlappingActors(collectedActors);

	for (size_t i = 0; i < collectedActors.Num(); ++i)
	{
		AActor* currentActor = collectedActors[i];
		AShot* currentShot = Cast<AShot>(currentActor);
		if (currentShot && currentShot->CanKillFoe)
		{
			IsActivated = true;
			HindranceMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//Destroy();
		}
	}
}

// Called every frame
void AHindranceRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsActivated)
	{
		HandleOverlap_Implementation();
	}
}

