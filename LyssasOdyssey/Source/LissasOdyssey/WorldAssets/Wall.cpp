// Fill out your copyright notice in the Description page of Project Settings.

#include "Wall.h"
#include "Components/SceneComponent.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//WallMesh ->CanCharacterStepUpOn = false;
	//CanCharacterStepUpOn = ECB_No;
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMesh->SetVisibility(!InvisibleWall);
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

