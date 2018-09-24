// Fill out your copyright notice in the Description page of Project Settings.

#include "Rabbit.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

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
	FVector direction = Lyssa->GetActorLocation() - GetActorLocation();
	direction = FVector(direction.X, direction.Y, 0);

	FRotator Rot = FRotationMatrix::MakeFromX(direction).Rotator();
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

		ShotCountdown = ShotInterval;
	}

	TArray<int32> indexes;
	indexes.Reset(0);
	for (size_t i = 0; i < Shots.Num(); i++)
	{
		AShot* shotTmp = Shots[i];
		if (shotTmp->ShouldBeDestroy)
			indexes.Add(i);
	}
	for (size_t i = 0; i < indexes.Num(); i++)
	{
		Shots[indexes[i]]->Destroy();
		Shots.RemoveAt(indexes[i]);
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
