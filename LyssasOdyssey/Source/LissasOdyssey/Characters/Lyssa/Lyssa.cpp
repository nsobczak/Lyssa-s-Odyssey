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
	MainCamera->RelativeLocation = FVector(-400, 0, 1000.0f);// Position the camera
	MainCamera->RelativeRotation.Pitch = 290.0f;
	MainCamera->bUsePawnControlRotation = false; // Allow the pawn to control rotation.

	initialPosZValue = GetActorLocation().Z;
}

// Called when the game starts or when spawned
void ALyssa::BeginPlay()
{
	Super::BeginPlay();

	// === Fylgja === 
	TArray<AActor*> Comps;
	GetAllChildActors(Comps);
	for (size_t i = 0; i < Comps.Num(); i++)
	{
		if (Comps[i]->IsA(AFylgja::StaticClass()))
			Fylgja = (AFylgja*)Comps[i];
	}
}

// Called every frame
void ALyssa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//keep same z value
	FVector loc = GetActorLocation();
	SetActorLocation(FVector(loc.X, loc.Y, initialPosZValue));
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

		// Set Character's rotation
		if (Value > 0)
		{
			this->GetMesh()->SetRelativeRotation(FRotator(0, 0, 0));
		}
		else
		{
			this->GetMesh()->SetRelativeRotation(FRotator(0, 180.0f, 0));
		}
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

		// Set Character's rotation
		if (Value > 0)
		{
			this->GetMesh()->SetRelativeRotation(FRotator(0, 90.0f, 0));
		}
		else
		{
			this->GetMesh()->SetRelativeRotation(FRotator(0, 270.0f, 0));
		}
	}
}


//____________________________________________________________________________________
#pragma endregion

