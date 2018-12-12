// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/ActorWithMeshBase.h"
#include "Pickup.generated.h"

UCLASS()
class LISSASODYSSEY_API APickup : public AActorWithMeshBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	///** return the mesh for the pickup*/
	//FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool newPickupState);

	/**function to call when the pickup is collected*/
	UFUNCTION(BlueprintNativeEvent)
		void WasCollected();
	virtual void WasCollected_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** true when pickup is used, and false when it is deactivated*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
		bool bIsActive;

};
