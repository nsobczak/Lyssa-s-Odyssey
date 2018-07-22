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
		shot->InitializeShot(ShotNature, ShotTTL, ShotSpeed, shotForwardOffset);
		Shots.Add(shot);

		TArray<int32> indexes;
		indexes.Reset(0);
		for (size_t i = 0; i < Shots.Num(); i++)
		{
			shot = Shots[i];
			if (shot->ShouldBeDestroy)
				indexes.Add(i);
		}
		for (size_t i = 0; i < indexes.Num(); i++)
		{
			Shots[indexes[i]]->Destroy();
			Shots.RemoveAt(indexes[i]);
		}

		ShotCountdown = ShotInterval;
	}
}

void ARabbit::CheckForDeath() {
	if (Life < 0.0f && !ShouldBeDestroyed)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Projectile hurts rabbit"));
		ShouldBeDestroyed = true;
	}
}

// Called every frame
void ARabbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookAtPlayer();

	ShotCountdown -= DeltaTime;
	HandleShots(DeltaTime);

	CheckForDeath();
}
