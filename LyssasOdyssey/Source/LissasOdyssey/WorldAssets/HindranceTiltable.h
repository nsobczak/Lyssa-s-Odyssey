// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hindrance.h"
#include "HindranceTiltable.generated.h"

UCLASS()
class LISSASODYSSEY_API AHindranceTiltable : public AHindrance
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHindranceTiltable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HandleOverlap_Implementation() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hindrance", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* HindranceSKMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hindrance", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ArrowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hindrance", meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* InvisibleWallLeft_ChildActorComponent;

	class AWall* InvisibleWallLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hindrance", meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* InvisibleWallRight_ChildActorComponent;

	class AWall* InvisibleWallRight;
};
