// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

UCLASS()
class LISSASODYSSEY_API AWall : public AActor
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

private:
	/** Static mesh to represent the wall in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wall", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* WallMesh;
};
