// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers/CustomTriggerGate.h"
#include "CustomTriggerGateWithCondition.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API ACustomTriggerGateWithCondition : public ACustomTriggerGate
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomTriggerGateWithCondition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerGate")
		bool TriggerWithTotalScoreInsteadOfCurrentScore = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnTriggerDetected_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerGate")
		int ScorePickupAmountToTrigger = 10;
};
