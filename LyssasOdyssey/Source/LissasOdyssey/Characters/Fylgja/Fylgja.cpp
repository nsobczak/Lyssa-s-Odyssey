#include "Fylgja.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"  
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h" 

// Sets default values
AFylgja::AFylgja(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	FylgjaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FylgjaMesh"));
	RootComponent = (USceneComponent*)FylgjaMesh;
}

// Called when the game starts or when spawned
void AFylgja::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFylgja::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FollowMousePosition();
}


void AFylgja::FollowMousePosition()
{
	//AFylgja *Character = Cast<AFylgja>(GetPawn());
	APlayerController* playercontroller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Get mouse position on screen
	float xMouse, yMouse;
	playercontroller->GetMousePosition(xMouse, yMouse);

	// Get Character position on screen
	FVector CharLoc = this->GetActorLocation();
	FVector2D CharInScreen;
	playercontroller->ProjectWorldLocationToScreen(CharLoc, CharInScreen);

	// Get mouse position relative to the Character.
	FVector2D Result;
	Result.X = -(yMouse - CharInScreen.Y);
	Result.Y = xMouse - CharInScreen.X;

	// Get angle rotation and rotation Character
	float angle = FMath::RadiansToDegrees(FMath::Acos(Result.X / Result.Size()));

	if (Result.Y < 0)
		angle = 360 - angle;

	FRotator rot(0, angle, 0);

	this->SetActorRotation(rot);
}