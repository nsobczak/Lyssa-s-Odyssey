// Fill out your copyright notice in the Description page of Project Settings.

#include "Shot.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Characters/Fylgja/Fylgja.h"
#include "Characters/Foes/Foe.h"


// Sets default values
AShot::AShot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	ShotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShotMesh"));
	RootComponent = (USceneComponent*)ShotMesh;
}

// Called when the game starts or when spawned
void AShot::BeginPlay()
{
	Super::BeginPlay();

	LyssaActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(this, 0));

	SpawningLocation = this->GetActorLocation();

	float ShotTimer = 0.0f;
}

void AShot::InitializeShot(int32 nature, float ttl, float speed)
{
	ShotTTL = ttl;
	ShotSpeed = speed;

	if (LyssaActor)
	{
		TargetDirection = (LyssaActor->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
		TargetDirection -= FVector(0, 0, TargetDirection.Z); //keep it moving in 2D plan
	}
	else
	{
		TargetDirection = FVector::ZeroVector;
	}

	SetActorLocation(GetActorLocation() + TargetDirection);//SetActorLocation(GetActorLocation() + TargetDirection * offset);
}

bool AShot::CustomDestroy()
{
	return Super::Destroy();
}

//________________________________________________________________________

//move towards player
void AShot::Move(float deltaTime)
{
	FVector newLocation = this->GetActorLocation() + TargetDirection * deltaTime * ShotSpeed;
	SetActorLocation(newLocation);
}

bool AShot::HandleOverlapWithFoe(AActor* currentActor)
{
	AFoe* currentFoe = Cast<AFoe>(currentActor);
	if (currentFoe && CanKillFoe)
	{
		//FString objectName = currentFoe->GetName();
		//UE_LOG(LogTemp, Log, TEXT("shot hurts %s"), *objectName);

		currentFoe->UpdateLife(-ShotDamage);
		currentFoe->SetCurrentState(ECharacterActionState::EOverlapProjectile);

		FString TheFloatStr = FString::SanitizeFloat(currentFoe->GetCurrentLife());
		TheFloatStr = TEXT("Projectile hurts Foe | life = ") + TheFloatStr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);

		CustomDestroy();
		return true;
	}
	else
	{
		return false;
	}
}

bool AShot::HandleOverlapWithLyssa(AActor* currentActor)
{
	ALyssa* currentLyssa = Cast<ALyssa>(currentActor);
	if (currentLyssa)
	{
		//FString objectName = currentLyssa->GetName();
		//UE_LOG(LogTemp, Log, TEXT("shot hurts %s"), *objectName);

		currentLyssa->UpdateLife(-ShotDamage);

		FString TheFloatStr = FString::SanitizeFloat(currentLyssa->GetCurrentLife());
		TheFloatStr = TEXT("Projectile hurts Lyssa | life = ") + TheFloatStr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);

		CustomDestroy();
		return true;
	}
	else
	{
		return false;
	}
}

bool AShot::HandleOverlapWithFylgja(AActor* currentActor)
{
	AFylgja* currentFylgja = Cast<AFylgja>(currentActor);
	if (currentFylgja)
	{
		//FString objectName = currentFylgja->GetName();
		//UE_LOG(LogTemp, Log, TEXT("shot hurts %s"), *objectName);

		//reverse direction 
		//TODO: change direction according to rotation
		FRotator rotF = currentFylgja->GetActorRotation();
		FVector fylgjaDir = currentFylgja->GetActorForwardVector().GetSafeNormal();// rotF.Vector().GetSafeNormal();
		TargetDirection = fylgjaDir;
		CanKillFoe = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Changed target direction"));

		return true;
	}
	else
	{
		return false;
	}
}

void AShot::HandleOverlap()
{
	//get overlaping actors and store them in an array
	TArray<AActor*> collectedActors;
	ShotMesh->GetOverlappingActors(collectedActors);

	for (size_t i = 0; i < collectedActors.Num(); ++i)
	{
		AActor* currentActor = collectedActors[i];
		if (!HandleOverlapWithFoe(currentActor))
		{
			if (!HandleOverlapWithLyssa(currentActor))
			{
				HandleOverlapWithFylgja(currentActor);
			}
		}
	}
}

// Called every frame
void AShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);

	HandleOverlap();

	ShotTimer += DeltaTime;
	if (ShotTimer > ShotTTL)
	{
		CustomDestroy();
	}
}

