// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Lyssa/Lyssa.h"
#include "Runtime/Core/Public/Containers/EnumAsByte.h"

#include "Shot.generated.h"


UENUM(BlueprintType, Category = "Shot")        //"BlueprintType" is essential specifier
enum UShotNature { Shot01 = 0, Shot02 };
UENUM(BlueprintType, Category = "Shot")
enum UShotDirection { ToPlayer = 0, Random };

UCLASS()
class LISSASODYSSEY_API AShot : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShot();

	void InitializeShot(UShotNature nature, UShotDirection newDirection, float ttl, float speed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		TSubclassOf<class AShot> BPShot;
	TEnumAsByte<UShotNature> ShotNature = UShotNature::Shot01;
	//UShotNature ShotNature = UShotNature::Shot01;

	float ShotSpeed = 100.0f;
	float ShotTTL = 2.0f;
	float ShotTimer = 0.0f;
	TEnumAsByte<UShotDirection> direction = UShotDirection::ToPlayer;
	//UShotDirection direction;

	float ShotDamage = 10.0f;

	void Move();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		ALyssa* Lyssa;

};
