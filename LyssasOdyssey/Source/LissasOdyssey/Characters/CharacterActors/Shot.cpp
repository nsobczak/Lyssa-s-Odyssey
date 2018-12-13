// Fill out your copyright notice in the Description page of Project Settings.

#include "Shot.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Characters/Fylgja/Fylgja.h"
#include "Characters/Foes/Foe.h"
#include "WorldAssets/Wall.h"


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

void AShot::InitializeShot(FVector ownerLocation, float ttl, float speed)
{
	ShotTTL = ttl;
	ShotSpeed = speed;

	if (LyssaActor)
	{
		TargetDirection = (this->GetActorLocation() - ownerLocation).GetSafeNormal();
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
	if (currentFoe && CanKillFoe && currentFoe->GetCurrentState() != ECharacterActionState::EDying)
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
	if (currentLyssa && !CanKillFoe)
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

		///*	Normalize both vectors
		//	calculate DOT product.Dot product is cosinus of those vectors.
		//	Calculate arcus cos of that value.*/
		//FVector oldNormalizedTargetDirection = TargetDirection;
		//float cosValue = FVector::DotProduct(currentFylgja->GetActorForwardVector(), oldNormalizedTargetDirection);
		//float angleAbsValue = acos(cosValue);
		//FRotator rotator = FRotator(0, 0, 2 * angleAbsValue);
		//TargetDirection = rotator.RotateVector(oldNormalizedTargetDirection);

		FVector normal = currentFylgja->GetActorForwardVector();
		FQuat betweenAngle = FQuat::FindBetween(TargetDirection.GetSafeNormal(), normal);
		FRotator rotator(betweenAngle);
		float angleToRotate = FMath::Abs(rotator.Yaw);
		//UE_LOG(Log, Warning, TEXT("rotator.Yaw= %f"), rotator.Yaw);

		float incidenceAng = 180.0f - angleToRotate; // 90.0f - complementaryAng;
		float newAngleToRotate = angleToRotate - FMath::Abs(incidenceAng) / 8.0f;
		float sign = rotator.Yaw > 0 ? 1.0f : -1.0f;
		rotator = FRotator(0, sign*newAngleToRotate, 0);

		TargetDirection = rotator.RotateVector(TargetDirection);

		/*float Ang1 = FMath::Atan2(TargetDirection.X, TargetDirection.Y);
		float Ang2 = FMath::Atan2(normal.X, normal.Y);
		float Ang = FMath::RadiansToDegrees(Ang1 - Ang2);
		if (Ang > 180.0f) Ang -= 360.0f;
		else if (Ang < -180.0f) Ang += 360.0f;*/

		CanKillFoe = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Changed target direction"));
		//UE_LOG(LogTemp, Log, TEXT("angleToRotate = %f \n incidence angle= %f \n newAngleToRotate= %f"),
		//	angleToRotate, incidenceAng, newAngleToRotate);

		return true;
	}
	else
	{
		return false;
	}
}

bool AShot::HandleOverlapWithWall(AActor* currentActor)
{
	AWall* currentWall = Cast<AWall>(currentActor);
	if (currentWall && currentWall->BlockShots)
	{
		//UE_LOG(LogTemp, Warning, TEXT("wall block shot"));
		CustomDestroy();
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
			if (!HandleOverlapWithFylgja(currentActor))
			{
				if (!HandleOverlapWithLyssa(currentActor))
				{
					HandleOverlapWithWall(currentActor);
				}
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

