// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "./Shot.h"
#include "../Lyssa/Lyssa.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
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

	UWorld* World;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		TEnumAsByte<UShotNature> ShotNature = UShotNature::Shot01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		TEnumAsByte<UShotDirection> ShotDirection = UShotDirection::ToPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		float ShotInterval = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		float ShotTTL = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		float ShotSpeed = 100.0f;

	float ShotCountdown = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		ALyssa* Lyssa;

};
