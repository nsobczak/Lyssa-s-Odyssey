// Fill out your copyright notice in the Description page of Project Settings.

#include "Rabbit.h"


// Sets default values
ARabbit::ARabbit(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARabbit::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
	Lyssa = (ALyssa*)UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	ShotCountdown = ShotInterval;
}

void ARabbit::LookAtPlayer()
{
	FRotator Rot = FRotationMatrix::MakeFromX(Lyssa->GetActorLocation() - GetActorLocation()).Rotator();
	SetActorRotation(Rot);
}

void ARabbit::HandleShots(float DeltaTime)
{
	if (BPShot == nullptr) { return; }

	if (ShotCountdown < 0.0f)
	{
		//TODO: use shotForwardOffset
		AShot* shot = (AShot*)GetWorld()->SpawnActor(BPShot);
		shot->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());
		shot->InitializeShot(ShotNature, ShotTTL, ShotSpeed);

		ShotCountdown = ShotInterval;
	}
}


// Called every frame
void ARabbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookAtPlayer();

	ShotCountdown -= DeltaTime;
	HandleShots(DeltaTime);
}
