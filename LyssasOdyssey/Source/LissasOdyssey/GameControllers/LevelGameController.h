// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Characters/Lyssa/Lyssa.h"
#include "../WorldAssets/FinishArea.h"

#include "LevelGameController.generated.h"

UCLASS()
class LISSASODYSSEY_API ALevelGameController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelGameController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		ALyssa* Lyssa;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		AFinishArea* FinishArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		bool IsLevelCompleted = false;

	UFUNCTION(BlueprintCallable, Category = "GameController")
		void CheckForLevelCompleted();

	//TODO: add infos here
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameController")
		float TimeLimit = 99.0f;
	
	float Timer = 0.0f;

};
