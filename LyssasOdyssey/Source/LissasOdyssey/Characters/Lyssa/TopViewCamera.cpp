// Fill out your copyright notice in the Description page of Project Settings.

#include "TopViewCamera.h"

// Sets default values
ATopViewCamera::ATopViewCamera()
{
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Load Shape_Sphere from the StarterContent
	FString assetPath = FString(TEXT("/Game/Ressources/sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> shapeSphereAsset(*assetPath);

	// StaticMesh root component
	UStaticMeshComponent* CameraTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraTarget"));
	RootComponent = static_cast<USceneComponent*>(CameraTarget);
	CameraTarget->SetCollisionProfileName(TEXT("NoCollision"));

	// Assign the shape to ourStaticMesh
	if (shapeSphereAsset.Succeeded())
	{
		CameraTarget->SetStaticMesh(shapeSphereAsset.Object);
	}
	// Log an error if the path cannot be found
	else
	{
		UE_LOG(LogScript, Error, TEXT("%s%s"), TEXT("Cannot load shape asset from path: "), *assetPath);
	}
}

// Called when the game starts or when spawned
void ATopViewCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATopViewCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


