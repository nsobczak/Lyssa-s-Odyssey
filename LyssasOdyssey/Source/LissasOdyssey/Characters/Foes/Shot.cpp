// Fill out your copyright notice in the Description page of Project Settings.

#include "Shot.h"


// Sets default values
AShot::AShot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AShot::InitializeShot(UShotNature nature, UShotDirection newDirection, float ttl, float speed)
{
	ShotTTL = ttl;
	ShotSpeed = speed;
	direction = newDirection;

	static ConstructorHelpers::FObjectFinder<UBlueprint> BPShotFinder(TEXT("/Game/Characters/Foes/Projectiles/Shots/Shot01/BPShot01"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint> BPShotFinder(TEXT("/Game/Characters/Foes/Projectiles/Shots/Shot02/BPShot02"));

	switch (nature)
	{
	case Shot01:
		if (BPShotFinder.Object)
			BPShot = (UClass*)BPShotFinder.Object->GeneratedClass;
		break;
	//case Shot02:
		//break;
	default:
		if (BPShotFinder.Object)
			BPShot = (UClass*)BPShotFinder.Object->GeneratedClass;
		break;
	}
}

void AShot::Move() {
	//TODO: move towards player

	FVector targetPoint;
	switch (direction)
	{
	case ToPlayer:
		targetPoint = (Lyssa->GetActorLocation() - this->GetActorLocation()) * 10.0f;
		break;
		//case Random:
		//break;
	default:
		targetPoint = (Lyssa->GetActorLocation() - this->GetActorLocation()) * 10.0f;
		break;
	}

	this->SetActorLocation((Lyssa->GetActorLocation() - this->GetActorLocation()) / 2, false);
}

// Called when the game starts or when spawned
void AShot::BeginPlay()
{
	Super::BeginPlay();

	Lyssa = (ALyssa*)UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	float ShotTimer = 0.0f;

	Move();
}

// Called every frame
void AShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ShotTimer += DeltaTime;
	if (ShotTimer > ShotTTL)
	{
		Destroy();
	}
}

