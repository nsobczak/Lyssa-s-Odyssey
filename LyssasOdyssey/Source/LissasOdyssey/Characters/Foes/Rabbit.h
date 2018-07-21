// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Lyssa/Lyssa.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Rabbit.generated.h"

UCLASS()
class LISSASODYSSEY_API ARabbit : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARabbit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LookAtPlayer();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//TODO: add infos here
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rabbit")
		float ShotInterval = 2.5f;

	float ShotTimer = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		ALyssa* Lyssa;
};
