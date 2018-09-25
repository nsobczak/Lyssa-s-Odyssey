// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Shot.generated.h"


//UENUM(BlueprintType, Category = "Shot")        //"BlueprintType" is essential specifier
//enum UShotNature { Shot01 = 0, Shot02 };
//UENUM(BlueprintType, Category = "Shot")
//enum UShotDirection { ToPlayer = 0, Random };

UCLASS()
class LISSASODYSSEY_API AShot : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShot();

	void InitializeShot(int32 nature, float ttl, float speed, float offset);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		TSubclassOf<class AShot> BPShot;
	int32 ShotNature = 0;

	float ShotSpeed = 10000.0f;
	float ShotTTL = 2.0f;
	float ShotTimer = 0.0f;
	bool ShouldBeDestroy = false;

	FVector targetDirection;

	float ShotDamage = 10.0f;
	bool CanKillFoe = false;

	void Move(float deltaTime);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		AActor* LyssaActor;
	FVector spawningLocation;
};
