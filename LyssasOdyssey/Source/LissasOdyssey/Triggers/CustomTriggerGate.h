// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers/CustomTriggerBase.h"
#include "CustomTriggerGate.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API ACustomTriggerGate : public ACustomTriggerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomTriggerGate();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerGate")
		TArray<class AGate*> GatesToControl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerGate")
		bool ShouldTriggerOpen = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnTriggerDetected_Implementation() override;

	UPROPERTY(VisibleAnywhere, Category = "TriggerGate")
		bool IsPlayerActorThatTriggers = true;

	bool UseConditionToTriggerGates = false;
};
