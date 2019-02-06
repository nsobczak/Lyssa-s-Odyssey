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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger_settings")
		bool CanBeTriggeredSeveralTimes = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger_settings")
		bool hasBeenTriggered = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Trigger_event")
		void OnTriggerDetected();
	virtual void OnTriggerDetected_Implementation();
	FName OnTriggerDetectedName = FName("OnTriggerDetected"); //for delay: BindUFunction

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger_settings")
		float DelayBfrTriggerDetection = 0.05f;

	UPROPERTY(VisibleAnywhere, Category = "Trigger_settings")
		class ATriggerBase* TriggerElement;

	UPROPERTY(VisibleAnywhere, Category = "Trigger_settings")
		AActor* ActorThatTriggers;

	UPROPERTY(VisibleAnywhere, Category = "Trigger_settings")
		bool IsTriggered = false;

private:
	/** Static mesh to represent the Mesh in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TriggerGate", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* TriggerCrossMesh;
};
