// Fill out your copyright notice in the Description page of Project Settings.

#include "HindranceRock.h"
#include "Engine/SkeletalMesh.h"

// Sets default values
AHindranceRock::AHindranceRock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the skeletal mesh component
	HindranceSKMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("HindranceSKMesh"));
	//HindranceSKMesh->SetupAttachment(RootComponent);
	//RootComponent->CreateOptionalDefaultSubobject
	//RootComponent = (USceneComponent*)HindranceSKMesh;
}

// Called when the game starts or when spawned
void AHindranceRock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHindranceRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

