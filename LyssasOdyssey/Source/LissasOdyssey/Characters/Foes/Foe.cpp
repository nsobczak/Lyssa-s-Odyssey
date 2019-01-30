// Fill out your copyright notice in the Description page of Project Settings.

#include "Foe.h"
#include "GameModes/LevelGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFoe::AFoe(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	FoeColliderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoeColliderMesh"));
	RootComponent = (USceneComponent*)FoeColliderMesh;

	//FoeSKMesh = CreateDefaultSubobject<USkeletalMesh>(TEXT("FoeSKMesh"));
	//RootComponent = (USceneComponent*)FoeSKMesh;
}

// Called when the game starts or when spawned
void AFoe::BeginPlay()
{
	Super::BeginPlay();

	Life = MaxLife;

	//set references
	World = GetWorld();
	Lyssa = Cast<ALyssa>(UGameplayStatics::GetPlayerPawn(this, 0));
	ShotCountdown = ShotInterval;

	//offset
	if (ShotOffset.Z == 1.0f)
	{
		ALevelGameMode* const currentGameMode = GetWorld()->GetAuthGameMode<ALevelGameMode>();
		ShotOffset = FVector(ShotOffset.X, ShotOffset.Y, currentGameMode->VerticalLevel);
	}

	FoeColliderMesh->SetVisibility(!InvisibleRootMesh);

	SetCurrentState(ECharacterActionState::EIdle);
	SetIsFoeActive(ShouldFoeStartActive);
}

bool AFoe::CustomDestroy()
{
	//TODO: use a timer to make dead body disappear after a while (use ShotCountdown to do so)
	return Super::Destroy();
}

float AFoe::GetCurrentLife()
{
	return Life;
}

void AFoe::UpdateLife(float lifeChange)
{
	Life += lifeChange;
	if (Life > MaxLife)
	{
		Life = MaxLife;
	}
}

bool AFoe::GetIsFoeActive()
{
	return this->IsFoeActive;
}

void AFoe::SetIsFoeActive(bool newState)
{
	this->IsFoeActive = newState;
}

//________________________________________________________________________

void AFoe::LookAtPlayer()
{
	//UE_LOG(LogTemp, Log, TEXT("LookAtPlayer"));

	FVector direction = Lyssa->GetActorLocation() - GetActorLocation();
	direction = FVector(direction.X, direction.Y, 0);

	FRotator Rot = FRotationMatrix::MakeFromX(direction).Rotator();
	SetActorRotation(Rot);
}

void AFoe::SpawnShots()
{
	if (BPShot == nullptr) { return; }

	FVector foeLocation = GetActorLocation();
	FVector shotLocation = FVector(foeLocation.X, foeLocation.Y, ShotOffset.Z);
	AShot* shot = (AShot*)GetWorld()->SpawnActor(BPShot);

	shot->SetActorLocationAndRotation(GetActorLocation()
		+ GetActorForwardVector() * ShotOffset.X + GetActorUpVector() * ShotOffset.Z, GetActorRotation());
	shot->InitializeShot(this->GetActorLocation(), ShotTTL, ShotSpeed);
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

	if (!IsFoeActive) { return; }

	//DrawDebugLine(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + this->GetActorForwardVector() * 1000.0f, FColor::Red);
	if (Life < 0.0f)
		SetCurrentState(ECharacterActionState::EDying);

	else if (CurrentState == ECharacterActionState::EOverlapProjectile) //intermediate state needed because state is changed by overlaping shot
	{
		//UE_LOG(LogTemp, Log, TEXT("ETakeDamage"));
		SetCurrentState(ECharacterActionState::ETakeDamage);
	}

	else if (CurrentState == ECharacterActionState::ETakeDamage)
		SetCurrentState(ECharacterActionState::EIdle);

	else if (FVector::DistSquared(GetActorLocation(), Lyssa->GetActorLocation()) < PlayerDetectionDistance)
	{
		if (ShouldLookAtPlayer)
			LookAtPlayer();

		ShotCountdown -= DeltaTime;
		if (ShotCountdown < 0.0f)
			SetCurrentState(ECharacterActionState::EAttack);
		else
			SetCurrentState(ECharacterActionState::EIdle);
	}

	else
		SetCurrentState(ECharacterActionState::EIdle);
}

#pragma region ActionStates

ECharacterActionState AFoe::GetCurrentState() const
{
	return CurrentState;
}

void AFoe::SetCurrentState(ECharacterActionState newState)
{
	CurrentState = newState;
	HandleNewState(CurrentState);
}

void AFoe::HandleNewState(ECharacterActionState newState)
{
	switch (newState)
	{
	case ECharacterActionState::EIdle:
		break;

	case ECharacterActionState::EAttack:
		SpawnShots();
		break;

	case ECharacterActionState::EOverlapProjectile:
		break;

	case ECharacterActionState::ETakeDamage:
		DamageEffect();
		break;

	case ECharacterActionState::EDying:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Projectile hurts foe"));
		//CustomDestroy();
		break;

	default:
		break;
	}
}

#pragma endregion
