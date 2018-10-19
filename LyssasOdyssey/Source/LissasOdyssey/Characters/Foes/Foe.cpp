// Fill out your copyright notice in the Description page of Project Settings.

#include "Foe.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AFoe::AFoe(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = (USceneComponent*)GetMesh();

	////create the static mesh component
	//FoeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoeMesh"));
	//RootComponent = (USceneComponent*)FoeMesh;
}

// Called when the game starts or when spawned
void AFoe::BeginPlay()
{
	Super::BeginPlay();

	//set references
	World = GetWorld();
	Lyssa = Cast<ALyssa>(UGameplayStatics::GetPlayerPawn(this, 0));
	ShotCountdown = ShotInterval;

	SetCurrentState(ECharacterActionState::EIdle);
}

bool AFoe::CustomDestroy()
{
	return Super::Destroy();
}

//________________________________________________________________________

void AFoe::LookAtPlayer()
{
	FVector direction = Lyssa->GetActorLocation() - GetActorLocation();
	direction = FVector(direction.X, direction.Y, 0);

	FRotator Rot = FRotationMatrix::MakeFromX(direction).Rotator();
	SetActorRotation(Rot);
}

void AFoe::HandleShots()
{
	if (BPShot == nullptr) { return; }

	FVector foeLocation = GetActorLocation();
	FVector shotLocation = FVector(foeLocation.X, foeLocation.Y, Lyssa->GetActorLocation().Z);
	AShot* shot = (AShot*)GetWorld()->SpawnActor(BPShot);

	shot->SetActorLocationAndRotation(GetActorLocation() + shotOffset, GetActorRotation()); //TODO: + shotVerticalOffset
	shot->InitializeShot(ShotNature, ShotTTL, ShotSpeed);
	Shots.Add(shot);

	ShotCountdown = ShotInterval;
}

void AFoe::CheckForDeath()
{
	if (Life < 0.0f)
	{
		SetCurrentState(ECharacterActionState::EDying);
	}
}

//________________________________________________________________________

// Called every frame
void AFoe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Life < 0.0f)
		SetCurrentState(ECharacterActionState::EDying);

	LookAtPlayer();

	ShotCountdown -= DeltaTime;
	if (ShotCountdown < 0.0f)
		SetCurrentState(ECharacterActionState::EAttack);
}



#pragma region ActionStates

ECharacterActionState AFoe::GetCurrentState() const
{
	return Currentstate;
}

void AFoe::SetCurrentState(ECharacterActionState newState)
{
	Currentstate = newState;
	HandleNewState(Currentstate);
}

void AFoe::HandleNewState(ECharacterActionState newState)
{
	switch (newState)
	{
	case ECharacterActionState::EIdle:
		break;

	case ECharacterActionState::EAttack:
		HandleShots();
		Currentstate = ECharacterActionState::EIdle;
		break;

	case ECharacterActionState::ETakeDamage:

		break;

	case ECharacterActionState::EDying:
		//TODO: add delay somehow for player to be able to see dying animation
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Projectile hurts foe"));
		CustomDestroy();
		break;

	default:
		break;
	}

}

#pragma endregion
