// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/ActorWithMeshBase.h"
#include "Wall.generated.h"

UCLASS()
class LISSASODYSSEY_API AWall : public AActorWithMeshBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
		bool InvisibleWall = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
		bool BlockShots = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
