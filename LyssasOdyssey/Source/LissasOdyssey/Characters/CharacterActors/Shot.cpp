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

	spawningLocation = this->GetActorLocation();

	float ShotTimer = 0.0f;
}

void AShot::InitializeShot(int32 nature, float ttl, float speed, float offset)
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

	SetActorLocation(GetActorLocation() + TargetDirection * offset);
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

//void AFylgja::HandleFylgjaReflect()
//{
//	//get overlaping actors and store them in an array
//	TArray<AActor*> collectedActors;
//	FylgjaMesh->GetOverlappingActors(collectedActors);
//
//	for (size_t i = 0; i < collectedActors.Num(); ++i)
//	{
//		AFoe* currentFoe = Cast<AFoe>(collectedActors[i]);
//		if (currentFoe)
//		{
//			FString objectName = currentFoe->GetName();
//			UE_LOG(LogTemp, Warning, TEXT("shot hurts %s"), *objectName);
//
//			currentFoe->UpdateLife(-40.0f);
//
//			FString TheFloatStr = FString::SanitizeFloat(currentFoe->GetCurrentLife());
//			TheFloatStr = TEXT("Projectile hurts Foe | life = ") + TheFloatStr;
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);
//
//			CustomDestroy();
//		}
//	}
//
//
//	for (size_t i = 0; i < Foes.Num(); i++)
//	{
//		AFoe* foe = Foes[i];
//		float sqrDist = FVector::DistSquared(foe->GetActorLocation(), Lyssa->GetActorLocation());
//
//		if (sqrDist < 200.0f * CollisionDistThreshold * CollisionDistThreshold)
//		{
//			//UE_LOG(LogTemp, Log, TEXT("if1"));
//			for (size_t j = 0; j < foe->Shots.Num(); j++)
//			{
//				AShot* shot = foe->Shots[j];
//
//				FRotator rotF = Lyssa->GetFylgja()->GetActorRotation();
//				FVector fylgjaDir = rotF.Vector().GetSafeNormal();
//
//				float sqrDistSF = FVector::DistSquared(shot->GetActorLocation(), Lyssa->GetFylgja()->GetActorLocation());
//				if (sqrDistSF < 1.5f *CollisionDistThreshold * CollisionDistThreshold)
//				{
//					shot->targetDirection = fylgjaDir;
//					shot->CanKillFoe = true;
//					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Changed target direction"));
//				}
//			}
//		}
//	}
//}

bool AShot::HandleOverlapWithFoe(AActor* currentActor)
{
	AFoe* currentFoe = Cast<AFoe>(currentActor);
	if (currentFoe && CanKillFoe)
	{
		FString objectName = currentFoe->GetName();
		UE_LOG(LogTemp, Warning, TEXT("shot hurts %s"), *objectName);

		currentFoe->UpdateLife(-40.0f);

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
		FString objectName = currentLyssa->GetName();
		UE_LOG(LogTemp, Warning, TEXT("shot hurts %s"), *objectName);

		currentLyssa->UpdateLife(-10.0f);

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
		FString objectName = currentFylgja->GetName();
		UE_LOG(LogTemp, Warning, TEXT("shot hurts %s"), *objectName);

		//reverse direction 
		//TODO: change direction according to rotation
		FRotator rotF = currentFylgja->GetActorRotation();
		FVector fylgjaDir = rotF.Vector().GetSafeNormal();
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

