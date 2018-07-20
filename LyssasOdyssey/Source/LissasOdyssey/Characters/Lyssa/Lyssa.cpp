// Fill out your copyright notice in the Description page of Project Settings.

#include "Lyssa.h"


// Sets default values
ALyssa::ALyssa(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// === CameraComponent === 
	//FPSCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	//FPSCamera->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//FPSCamera->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);// Position the camera a bit above the eyes
	//FPSCamera->RelativeRotation.Pitch = 0;
	//FPSCamera->bUsePawnControlRotation = true; // Allow the pawn to control rotation.

}

// Called when the game starts or when spawned
void ALyssa::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALyssa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALyssa::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

