// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hindrance.generated.h"

UCLASS()
class LISSASODYSSEY_API AHindrance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHindrance();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hindrance")
		bool BlockShots = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hindrance")
		bool IsActivated = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Hindrance_event")
		void HandleOverlap();
	virtual void HandleOverlap_Implementation();

};
