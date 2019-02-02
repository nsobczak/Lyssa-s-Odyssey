// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ActorWithMeshBase.generated.h"

UCLASS()
class LISSASODYSSEY_API AActorWithMeshBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActorWithMeshBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Static mesh to represent the wall in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BaseMesh;

};
