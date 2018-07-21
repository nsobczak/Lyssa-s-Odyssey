// Fill out your copyright notice in the Description page of Project Settings.

#include "Lyssa.h"


// Sets default values
ALyssa::ALyssa(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();


	// === CameraComponent === 
	MainCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("MainCamera"));
	MainCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MainCamera->RelativeLocation = FVector(-400, 0, 1500.0f);// Position the camera
	MainCamera->RelativeRotation.Pitch = 290.0f; //TODO: make it look to the ground
	MainCamera->bUsePawnControlRotation = false; // Allow the pawn to control rotation.

	// === Fylgya === 
	TArray<AActor*> childActors;
	this->GetAllChildActors(childActors);
	UE_LOG(LogTemp, Warning, TEXT("childActors.Num() = %i"), childActors.Num());

	for (size_t i = 0; i < childActors.Num(); i++)
	{
		FString objectName = childActors[i]->GetName();
		UE_LOG(LogTemp, Warning, TEXT("child: %s"), *objectName);
		if (childActors[i]->IsA(AFylgja::StaticClass())) {
			Fylgya = (AFylgja*)Children[i];
			UE_LOG(LogTemp, Warning, TEXT("Fylgya = (AFylgja*)Children[i]"));
		}

	}
	if (Fylgya)
	{
		FString objectName = Fylgya->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s was assigned"), *objectName);
	}
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

#pragma region Input
//____________________________________________________________________________________

// Called to bind functionality to input
void ALyssa::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveUp", this, &ALyssa::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALyssa::MoveRight);
}

void ALyssa::MoveUp(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ALyssa::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


//____________________________________________________________________________________
#pragma endregion

