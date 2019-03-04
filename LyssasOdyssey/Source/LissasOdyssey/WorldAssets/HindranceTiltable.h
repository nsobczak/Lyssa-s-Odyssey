// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HindranceTiltable.generated.h"

UCLASS()
class LISSASODYSSEY_API AHindranceTiltable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHindranceTiltable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hindrance")
		bool BlockShots = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hindrance")
		bool isTilted = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hindrance", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* HindranceSKMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hindrance", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ArrowMesh;

	UFUNCTION()
		void HandleOverlap();
};
