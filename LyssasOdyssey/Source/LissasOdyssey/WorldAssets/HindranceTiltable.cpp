// Fill out your copyright notice in the Description page of Project Settings.

#include "HindranceTiltable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/CharacterActors/Shot.h"


// Sets default values
AHindranceTiltable::AHindranceTiltable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HindranceSKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HindranceSKMesh"));
	RootComponent = (USceneComponent*)HindranceSKMesh;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->AttachToComponent(HindranceSKMesh, FAttachmentTransformRules::KeepRelativeTransform);
}


// Called when the game starts or when spawned
void AHindranceTiltable::BeginPlay()
{
	Super::BeginPlay();
	
	ArrowMesh->SetVisibility(false);
	HindranceSKMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
}


void AHindranceTiltable::HandleOverlap()
{
	//get overlaping actors and store them in an array
	TArray<AActor*> collectedActors;
	HindranceSKMesh->GetOverlappingActors(collectedActors);

	for (size_t i = 0; i < collectedActors.Num(); ++i)
	{
		AActor* currentActor = collectedActors[i];
		AShot* currentShot = Cast<AShot>(currentActor);
		if (currentShot && currentShot->CanKillFoe)
		{
			isTilted = true;
			HindranceSKMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
			//HindranceSKMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}


// Called every frame
void AHindranceTiltable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isTilted)
	{
		HandleOverlap();
	}
}

