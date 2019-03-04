// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hindrance.h"
//#include "DestructibleComponent.h"
#include "HindranceRock.generated.h"

UCLASS()
class LISSASODYSSEY_API AHindranceRock : public AHindrance
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

	void HandleOverlap_Implementation() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* HindranceMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		//UDestructibleMesh* HindranceDM;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		//USkeletalMesh* HindranceSKMesh;
};
