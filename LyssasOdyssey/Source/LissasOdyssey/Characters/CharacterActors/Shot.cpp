// Fill out your copyright notice in the Description page of Project Settings.

#include "Shot.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AShot::AShot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	ShotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShotMesh"));
	RootComponent = (USceneComponent*)ShotMesh;
}

// Called when the game starts or when spawned
void AShot::BeginPlay()
{
	Super::BeginPlay();

	LyssaActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(this, 0));

	spawningLocation = this->GetActorLocation();

	float ShotTimer = 0.0f;
}

void AShot::InitializeShot(int32 nature, float ttl, float speed, float offset)
{
	ShotTTL = ttl;
	ShotSpeed = speed;

	if (LyssaActor)
	{
		targetDirection = (LyssaActor->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
		targetDirection -= FVector(0, 0, targetDirection.Z); //keep it moving in 2D plan
	}
	else
	{
		targetDirection = FVector::ZeroVector;
	}

	SetActorLocation(GetActorLocation() + targetDirection * offset);
}

//move towards player
void AShot::Move(float deltaTime)
{
	FVector newLocation = this->GetActorLocation() + targetDirection * deltaTime * ShotSpeed;
	SetActorLocation(newLocation);
}

// Called every frame
void AShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);

	ShotTimer += DeltaTime;
	if (ShotTimer > ShotTTL && !ShouldBeDestroy)
	{
		ShouldBeDestroy = true;
	}
}


