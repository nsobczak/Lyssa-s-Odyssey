// Fill out your copyright notice in the Description page of Project Settings.

#include "Rabbit.h"


// Sets default values
ARabbit::ARabbit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARabbit::BeginPlay()
{
	Super::BeginPlay();

	Lyssa = (ALyssa*)UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
}

void ARabbit::LookAtPlayer()
{
	FRotator Rot = FRotationMatrix::MakeFromX(Lyssa->GetActorLocation() - GetActorLocation()).Rotator();
	SetActorRotation(Rot);
}

// Called every frame
void ARabbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookAtPlayer();
}

