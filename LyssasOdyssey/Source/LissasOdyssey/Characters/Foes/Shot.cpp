// Fill out your copyright notice in the Description page of Project Settings.

#include "Shot.h"


// Sets default values
AShot::AShot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AShot::InitializeShot(int32 nature, float ttl, float speed)
{
	ShotTTL = ttl;
	ShotSpeed = speed;

	targetDirection = (Lyssa->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
	targetDirection -= FVector(0, 0, targetDirection.Z);
}

// Called when the game starts or when spawned
void AShot::BeginPlay()
{
	Super::BeginPlay();

	Lyssa = (ALyssa*)UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	spawningLocation = this->GetActorLocation();

	float ShotTimer = 0.0f;
}

//move towards player
void AShot::Move(float deltaTime) {
	FVector newLocation = this->GetActorLocation() + targetDirection * deltaTime * ShotSpeed;
	SetActorLocation(newLocation);

}
// Called every frame
void AShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);

	ShotTimer += DeltaTime;
	if (ShotTimer > ShotTTL)
	{
		Destroy();
	}
}


