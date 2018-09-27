// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Shot.generated.h"


//UENUM(BlueprintType, Category = "Shot")        //"BlueprintType" is essential specifier
//enum UShotNature { Shot01 = 0, Shot02 };
//UENUM(BlueprintType, Category = "Shot")
//enum UShotDirection { ToPlayer = 0, Random };

UCLASS()
class LISSASODYSSEY_API AShot : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShot();

	UFUNCTION(BlueprintCallable, Category = "Shot")
		void InitializeShot(int32 nature, float ttl, float speed, float offset);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		TSubclassOf<class AShot> BPShot;
	int32 ShotNature = 0;

	UFUNCTION(BlueprintCallable, Category = "Shot")
		bool CustomDestroy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot")
		float ShotSpeed = 10000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot")
		float ShotTTL = 2.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shot")
		float ShotTimer = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shot")
		FVector TargetDirection;

	float ShotDamage = 10.0f;
	bool CanKillFoe = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		AActor* LyssaActor;
	FVector spawningLocation;

private:
	/** Static mesh to represent the Shot in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shot", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ShotMesh;

	UFUNCTION()
		void Move(float deltaTime);

	UFUNCTION()
		bool HandleOverlapWithFoe(AActor * currentActor);
	UFUNCTION()
		bool HandleOverlapWithLyssa(AActor * currentActor);
	UFUNCTION()
		bool HandleOverlapWithFylgja(AActor* currentActor);
	void HandleOverlap();
};
