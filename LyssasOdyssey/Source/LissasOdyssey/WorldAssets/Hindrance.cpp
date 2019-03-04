// Fill out your copyright notice in the Description page of Project Settings.

#include "Hindrance.h"

// Sets default values
AHindrance::AHindrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHindrance::BeginPlay()
{
	Super::BeginPlay();

	IsActivated = false;
}

void AHindrance::HandleOverlap_Implementation()
{
	//HandleOverlap_Implementation
}

// Called every frame
void AHindrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

