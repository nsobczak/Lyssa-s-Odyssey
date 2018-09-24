// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Characters/CharacterActors/Shot.h"
#include "Characters/Lyssa/Lyssa.h"

#include "Rabbit.generated.h"

UCLASS()
class LISSASODYSSEY_API ARabbit : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARabbit(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LookAtPlayer();

	void HandleShots(float DeltaTime);

	void CheckForDeath();

	UWorld* World;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		float Life = 40.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Foe")
		bool ShouldBeDestroyed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		TSubclassOf<class AShot> BPShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rabbit")
		TArray<AShot*> Shots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		float shotForwardOffset = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		int32 ShotNature = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		float ShotInterval = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		float ShotTTL = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		float ShotSpeed = 3000.0f;

	float ShotCountdown = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		ALyssa* Lyssa;

};
