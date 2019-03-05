// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers/CustomTriggerGate.h"
#include "CustomTriggerGateWithLocks.generated.h"

/**
 * 
 */
UCLASS()
class LISSASODYSSEY_API ACustomTriggerGateWithLocks : public ACustomTriggerGate
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACustomTriggerGateWithLocks();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnTriggerDetected_Implementation() override;
};
