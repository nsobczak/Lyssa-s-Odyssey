// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotateOverTime.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LISSASODYSSEY_API URotateOverTime : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URotateOverTime();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation")
		FRotator RotatorAnglePerSecond = FRotator(0, 100.0f, 0);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


};
