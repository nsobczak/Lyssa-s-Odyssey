// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Characters/CharacterActors/Shot.h"
#include "Characters/Lyssa/Lyssa.h"

#include "Foe.generated.h"

UCLASS()
class LISSASODYSSEY_API AFoe : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFoe(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Foe")
		float GetCurrentLife();

	/**	function to update the foe's life
	* @param powerChange This is the amount to change the power by, can be positive or negative
	*/
	UFUNCTION(BlueprintCallable, Category = "Foe")
		void UpdateLife(float lifeChange);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Foe")
		bool ShouldBeDestroyed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		TSubclassOf<class AShot> BPShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Foe")
		TArray<AShot*> Shots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		float shotForwardOffset = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		int32 ShotNature = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		float ShotInterval = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		float ShotTTL = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		float ShotSpeed = 3000.0f;

	float ShotCountdown = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		ALyssa* Lyssa;

protected:
	void LookAtPlayer();

	void HandleShots(float DeltaTime);

	void CheckForDeath();

	UWorld* World;

	//BlueprintImplementableEvent = we don't need to define function in code at all, just call it and let the blueprint define it
	UFUNCTION(BlueprintImplementableEvent, Category = "Foe")
		void LifeChangeEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Foe")
		void DeathEffect();

private:
	/** Static mesh to represent the foe in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Foe", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* FoeMesh;

	UPROPERTY(VisibleAnywhere, Category = "Foe")
		float Life = 40.0f;

};
