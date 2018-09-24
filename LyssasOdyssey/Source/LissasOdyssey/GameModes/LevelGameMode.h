// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameModes/MainGameMode.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Characters/Foes/Rabbit.h"
#include "WorldAssets/FinishArea.h"

#include "LevelGameMode.generated.h"

UCLASS()
class LISSASODYSSEY_API ALevelGameMode : public AMainGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		float DamageRate = 100.0f;
	float DamageRateTimer = 0.0f;

	void HandleFylgjaReflect();
	void HandleProjectileDamage();

	void CheckForDeath();

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		AFinishArea* FinishArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		ALyssa* Lyssa;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		TArray<ARabbit*> Rabbits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameController")
		float CollisionDistThreshold = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		bool IsLevelCompleted = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		bool IsGameOver = false;

	UFUNCTION(BlueprintCallable, Category = "GameController")
		void CheckForLevelCompleted();

	//TODO: add infos here
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameController")
		float TimeLimit = 99.0f;

	float Timer = 0.0f;

};
