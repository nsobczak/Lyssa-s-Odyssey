// Fill out your copyright notice in the Description page of Project Settings.

#include "MinimapCorner.h"


// Sets default values
AMinimapCorner::AMinimapCorner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh->SetMobility(EComponentMobility::Static);
	BaseMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	BaseMesh->SetGenerateOverlapEvents(false);
}

CornerNature AMinimapCorner::GetNature()
{
	return Nature;
}

// Called when the game starts or when spawned
void AMinimapCorner::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMesh->SetVisibility(!ShouldMeshBeVisible);
}

//// Called every frame
//void AMinimapCorner::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

