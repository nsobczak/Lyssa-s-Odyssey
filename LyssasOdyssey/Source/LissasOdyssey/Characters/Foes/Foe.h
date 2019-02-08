// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Characters/CharacterActors/Shot.h"
#include "Characters/Lyssa/Lyssa.h"

#include "Foe.generated.h"

//enum to store the current state of character
UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	EIdle,
	EAttack,
	EOverlapProjectile,
	ETakeDamage,
	EDying
	//,EWalk//not needed if we use morph with idle state?
};

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

#pragma region Life
public:
	UFUNCTION(BlueprintPure, Category = "Life")
		float GetCurrentLife();

	/**	function to update the foe's life
	* @param powerChange This is the amount to change the power by, can be positive or negative
	*/
	UFUNCTION(BlueprintCallable, Category = "Life")
		void UpdateLife(float lifeChange);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
		float MaxLife = 100.0f;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Life")
		float Life;

#pragma endregion

public:
	UFUNCTION(BlueprintPure, Category = "Foe")
		ECharacterActionState GetCurrentState()const;

	void SetCurrentState(ECharacterActionState newState);

	UFUNCTION(BlueprintPure, Category = "Foe")
		bool GetIsFoeActive();

	UFUNCTION(BlueprintCallable, Category = "Foe")
		void SetIsFoeActive(bool newState);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foe")
		bool ShouldFoeStartActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foe")
		float DestroyCountdown = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foe")
		float PlayerDetectionDistance = 20000000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foe")
		float PlayerCollisionDamage = 20.0f;

#pragma region Shots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoeShot")
		TSubclassOf<class AShot> BPShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FoeShot")
		TArray<AShot*> Shots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoeShot")
		FVector ShotOffset = FVector(500.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoeShot")
		float ShotInterval = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoeShot")
		float ShotTTL = 8.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoeShot")
		float ShotSpeed = 3000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FoeShot")
		float ShotCountdown = 0.0f;
#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		ALyssa* Lyssa;

protected:
	UFUNCTION()
		void MainFoe(float DeltaTime, bool isUnderPlayerDetectionDistance);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
		class ALevelGameMode* CurrentLGameMode;

	UFUNCTION(BlueprintCallable, Category = "Foe")
		virtual bool CustomDestroy();

	UFUNCTION(BlueprintCallable, Category = "Foe")
		void CheckForPlayerOverlap(float deltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foe")
		float PlayerOverlapCheckRate = 0.25f;

	float OverlapTimer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		bool IsRootMeshInvisible = true;

	UPROPERTY(VisibleAnywhere, Category = "Foe")
		bool IsFoeActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		bool ShouldLookAtPlayer = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foe")
		FVector DefaultShotDirection = FVector(1.0f, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foe")
		float DelayBfrDestroyBody = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foe")
		bool UsePlayerDetectionDistance = true;

	void LookAtPlayer();

	void SpawnShots();

	void CheckForDeath();

	UWorld* World;

	//BlueprintImplementableEvent = we don't need to define function in code at all, just call it and let the blueprint define it
	UFUNCTION(BlueprintImplementableEvent, Category = "Foe")
		void DamageEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Foe")
		void DeathEffect();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foe", meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* FoeSKMesh;

	/** Static mesh to represent the foe collider*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Foe", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* FoeColliderMesh;

	UPROPERTY(VisibleAnywhere, Category = "Foe")
		ECharacterActionState CurrentState;

	void HandleNewState(ECharacterActionState newState);
};
