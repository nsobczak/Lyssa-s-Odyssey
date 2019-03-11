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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gate")
		bool IsInitialized = false;

	UFUNCTION(BlueprintCallable, Category = "Gate")
		TArray<class AGateLock*> GetLocks();

	UFUNCTION(BlueprintCallable, Category = "Gate")
		void AddLock(class AGateLock* locktoAdd);

	UFUNCTION(BlueprintCallable, Category = "Gate")
		bool BreakLock(class AGateLock* locktoBreak);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Gate")
		bool IsOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate")
		bool useBlueLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate")
		bool useGreenLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate")
		bool usePurpleLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate")
		bool useRedLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate")
		bool useYellowLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate")
		TArray<class AGateLock*> Locks;
};
