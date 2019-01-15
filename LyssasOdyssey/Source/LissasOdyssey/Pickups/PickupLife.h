// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "PickupLife.generated.h"

/**
 * 
 */
UCLASS()
class LISSASODYSSEY_API APickupLife : public APickup
{
	GENERATED_BODY()
	
	
public:
	// Sets default values for this actor's properties
	APickupLife();

	/**Override the wasCollected function, use Implementation cause it's a blueprint native event*/
	void WasCollected_Implementation() override;

	float GetLifeAmount();

protected:
	///**set the amount of power the battery gives to the character*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	//	float BatteryPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
		float LifeAmount = 20.0f;
	
};
