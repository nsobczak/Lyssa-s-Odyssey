// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "Utils/GameEnums.h"
#include "PickupKey.generated.h"

/**
 * 
 */
UCLASS()
class LISSASODYSSEY_API APickupKey : public APickup
{
	GENERATED_BODY()


public:
	APickupKey();

	/**Override the wasCollected function, use Implementation cause it's a blueprint native event*/
	void WasCollected_Implementation() override;

	TEnumAsByte<KeyLockNature> GetKeyNature();

protected:
	UPROPERTY(EditAnywhere, Category = "Key", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<KeyLockNature> KeyNature = KeyLockNature::YELLOW;
};
