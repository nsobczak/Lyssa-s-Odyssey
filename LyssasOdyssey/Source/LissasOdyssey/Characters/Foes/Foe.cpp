// Fill out your copyright notice in the Description page of Project Settings.

#include "Foe.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AFoe::AFoe(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	FoeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoeMesh"));
	RootComponent = (USceneComponent*)FoeMesh;
}

// Called when the game starts or when spawned
void AFoe::BeginPlay()
{
	Super::BeginPlay();

	//set references
	World = GetWorld();
	Lyssa = Cast<ALyssa>(UGameplayStatics::GetPlayerPawn(this, 0));
	ShotCountdown = ShotInterval;
}

float AFoe::GetCurrentLife()
{
	return Life;
}

void AFoe::UpdateLife(float lifeChange)
{
	Life += lifeChange;
}

//________________________________________________________________________

void AFoe::LookAtPlayer()
{
	FVector direction = Lyssa->GetActorLocation() - GetActorLocation();
	direction = FVector(direction.X, direction.Y, 0);

	FRotator Rot = FRotationMatrix::MakeFromX(direction).Rotator();
	SetActorRotation(Rot);
}

void AFoe::HandleShots(float DeltaTime)
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

void AFoe::CheckForDeath() {
	if (Life < 0.0f && !ShouldBeDestroyed)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Projectile hurts foe"));
		ShouldBeDestroyed = true;
	}
}

// Called every frame
void AFoe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookAtPlayer();

	ShotCountdown -= DeltaTime;
	HandleShots(DeltaTime);

	CheckForDeath();
}
