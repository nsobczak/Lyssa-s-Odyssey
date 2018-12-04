// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomTriggerBase.generated.h"

UCLASS()
class LISSASODYSSEY_API ACustomTriggerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomTriggerBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Trigger_event")
		void OnTriggerDetected();
	virtual void OnTriggerDetected_Implementation();

	UPROPERTY(EditAnywhere, Category = "Trigger_settings")
		class ATriggerBase* TriggerElement;


	UPROPERTY(EditAnywhere, Category = "Trigger_settings")
		AActor* ActorThatTriggers;
};
