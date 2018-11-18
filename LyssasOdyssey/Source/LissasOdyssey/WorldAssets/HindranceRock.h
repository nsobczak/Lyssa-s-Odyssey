// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HindranceRock.generated.h"

UCLASS()
class LISSASODYSSEY_API AHindranceRock : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHindranceRock();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMesh* HindranceSKMesh;
};
