// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers/CustomTriggerBase.h"
#include "CustomTriggerFylgja.generated.h"

/**
 * 
 */
UCLASS()
class LISSASODYSSEY_API ACustomTriggerFylgja : public ACustomTriggerBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACustomTriggerFylgja();

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerFylgja")
		bool TriggerFylgja = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnTriggerDetected_Implementation() override;

	UPROPERTY(VisibleAnywhere, Category = "TriggerFylgja")
		bool IsPlayerActorThatTriggers = true;
};
