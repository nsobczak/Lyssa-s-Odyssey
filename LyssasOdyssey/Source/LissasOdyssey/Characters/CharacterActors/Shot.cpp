// Fill out your copyright notice in the Description page of Project Settings.

#include "Shot.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameModes/LevelGameMode.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Characters/Fylgja/Fylgja.h"
#include "Characters/Foes/Foe.h"
#include "WorldAssets/Wall.h"
#include "WorldAssets/Hindrance.h"


// Sets default values
AShot::AShot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	ShotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShotMesh"));
	RootComponent = (USceneComponent*)ShotMesh;

	OnActorBeginOverlap.AddDynamic(this, &AShot::HandleOverlap);
	if (DEBUG) UE_LOG(LogTemp, Log, TEXT("%s binded AddDynamic"), *(this->GetName()));
}

// Called when the game starts or when spawned
void AShot::BeginPlay()
{
	Super::BeginPlay();

	LyssaActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(this, 0));

	SpawningLocation = this->GetActorLocation();

	CurrentLGameMode = (ALevelGameMode*)GetWorld()->GetAuthGameMode();
	if (!CurrentLGameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentMainGameMode is null"));
	}
}

void AShot::InitializeShot(FVector ownerLocation, float ttl, float speed)
{
	ShotTTL = ttl;
	ShotSpeed = speed;

	TargetDirection = (this->GetActorLocation() - ownerLocation).GetSafeNormal();
	TargetDirection -= FVector(0, 0, TargetDirection.Z); //keep it moving in 2D plan

	SetActorLocation(GetActorLocation() + TargetDirection);//SetActorLocation(GetActorLocation() + TargetDirection * offset);
	ShotAttack();
}

void AShot::InitializeShot(FVector ownerLocation, FVector targetDirection, float ttl, float speed)
{
	ShotTTL = ttl;
	ShotSpeed = speed;

	TargetDirection = targetDirection.GetSafeNormal();

	SetActorLocation(GetActorLocation() + TargetDirection);//SetActorLocation(GetActorLocation() + TargetDirection * offset);
	ShotAttack();
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
		ShotHitFoe();

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
	if (currentLyssa && CanKillPlayer)
	{
		ShotHitLyssa();

		//FString objectName = currentLyssa->GetName();
		//UE_LOG(LogTemp, Log, TEXT("shot hurts %s"), *objectName);

		currentLyssa->UpdateLife(-ShotDamage);
		currentLyssa->DamageEffect();

		FString TheFloatStr = FString::SanitizeFloat(currentLyssa->GetCurrentLife());
		TheFloatStr = TEXT("Projectile hurts Lyssa | life = ") + TheFloatStr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);

		CustomDestroy();
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("%s: passed custom destroy in HandleOverlapWithLyssa"), *(this->GetName()));
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
	if (currentFylgja && currentFylgja->IsVisible)
	{
		ShotReflection();

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
		CanKillPlayer = false;
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

bool AShot::HandleOverlapWithHindrance(AActor* currentActor)
{
	AHindrance* currentHindrance = Cast<AHindrance>(currentActor);

	if (currentHindrance && currentHindrance->BlockShots && !CanKillFoe)
	{
		//UE_LOG(LogTemp, Warning, TEXT("hindrance blocked shot"));
		CustomDestroy();
		return true;
	}
	else
	{
		return false;
	}
}

void AShot::HandleOverlap(AActor* overlappedActor, AActor* otherActor)
{
	////get overlaping actors and store them in an array
	//TArray<AActor*> collectedActors;
	//ShotMesh->GetOverlappingActors(collectedActors);

	//for (size_t i = 0; i < collectedActors.Num(); ++i)
	//{
	//	AActor* currentActor = collectedActors[i];
	//	if (!HandleOverlapWithFoe(currentActor))
	//	{
	//		if (!HandleOverlapWithFylgja(currentActor))
	//		{
	//			if (!HandleOverlapWithLyssa(currentActor))
	//			{
	//				if (!HandleOverlapWithWall(currentActor))
	//				{
	//					HandleOverlapWithHindrance(currentActor);
	//				}
	//			}
	//		}
	//	}
	//}

	if (DEBUG)  UE_LOG(LogTemp, Log, TEXT("%s HandleOverlap"), *(this->GetName()));

	if (CurrentLGameMode && CurrentLGameMode->GetIsBeginFunctionCompleted() && overlappedActor && otherActor)
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("%s: overlappedActor && otherActor not null"), *(this->GetName()));
		if (!HandleOverlapWithFoe(otherActor))
		{
			if (DEBUG) UE_LOG(LogTemp, Log, TEXT("%s: after HandleOverlapWithFoe"), *(this->GetName()));
			if (!HandleOverlapWithFylgja(otherActor))
			{
				if (DEBUG) UE_LOG(LogTemp, Log, TEXT("%s: after HandleOverlapWithFylgja"), *(this->GetName()));
				if (!HandleOverlapWithLyssa(otherActor))
				{
					if (DEBUG) UE_LOG(LogTemp, Log, TEXT("%s: after HandleOverlapWithLyssa"), *(this->GetName()));
					if (!HandleOverlapWithWall(otherActor))
					{
						if (DEBUG) UE_LOG(LogTemp, Log, TEXT("%s: after HandleOverlapWithWall"), *(this->GetName()));
						HandleOverlapWithHindrance(otherActor);
					}
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

	//HandleOverlap();

	ShotTimer += DeltaTime;
	if (ShotTimer > ShotTTL)
	{
		CustomDestroy();
	}
}

