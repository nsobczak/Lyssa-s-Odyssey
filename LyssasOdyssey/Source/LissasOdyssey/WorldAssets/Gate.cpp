// Fill out your copyright notice in the Description page of Project Settings.

#include "Gate.h"

// Sets default values
AGate::AGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();

	this->IsOpen = StartOpen;
}

bool AGate::GetIsOpen()
{
	return this->IsOpen;
}

void AGate::SetIsOpen(bool newState)
{
	this->IsOpen = newState;
}

void AGate::ChangeIsOpen()
{
	this->IsOpen = !this->IsOpen;
}


// Called every frame
void AGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

