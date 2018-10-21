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
	//,EWalk
};

UCLASS()
class LISSASODYSSEY_API AFoe : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFoe(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Foe")
		ECharacterActionState GetCurrentState()const;

	void SetCurrentState(ECharacterActionState newState);

	UFUNCTION(BlueprintCallable, Category = "Foe")
		bool CustomDestroy();

#pragma region Shots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoeShot")
		TSubclassOf<class AShot> BPShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FoeShot")
		TArray<AShot*> Shots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoeShot")
		FVector shotOffset = FVector(0, 500.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoeShot")
		int32 ShotNature = 0;
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
	void LookAtPlayer();

	void HandleShots();

	void CheckForDeath();

	UWorld* World;

	//BlueprintImplementableEvent = we don't need to define function in code at all, just call it and let the blueprint define it
	UFUNCTION(BlueprintImplementableEvent, Category = "Foe")
		void LifeChangeEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Foe")
		void DeathEffect();

private:
	UPROPERTY(VisibleAnywhere, Category = "Foe")
		ECharacterActionState Currentstate;

	void HandleNewState(ECharacterActionState newState);

};
