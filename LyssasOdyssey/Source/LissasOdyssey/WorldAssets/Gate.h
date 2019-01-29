// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gate.generated.h"

UCLASS()
class LISSASODYSSEY_API AGate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGate();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Gate")
		bool GetIsOpen();

	UFUNCTION(BlueprintCallable, Category = "Gate")
		void SetIsOpen(bool newState);

	UFUNCTION(BlueprintCallable, Category = "Gate")
		void ChangeIsOpen();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gate")
		bool StartOpen = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Gate")
		bool IsOpen = false;
};
