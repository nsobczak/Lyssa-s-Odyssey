// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers/CustomTriggerBase.h"
#include "CustomTriggerFoe.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API ACustomTriggerFoe : public ACustomTriggerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomTriggerFoe();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerFoe")
		TArray<class AFoe*> FoesToControl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerFoe")
		bool ShouldActivateFoes = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnTriggerDetected_Implementation() override;

	UPROPERTY(VisibleAnywhere, Category = "TriggerFoe")
		bool IsPlayerActorThatTriggers = true;

	UPROPERTY(VisibleAnywhere, Category = "TriggerFoe")
		class ALevelGameMode* LevelGameMode;
};
