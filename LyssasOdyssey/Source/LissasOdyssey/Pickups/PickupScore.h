// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "PickupScore.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API APickupScore : public APickup
{
	GENERATED_BODY()



public:
	APickupScore();

	/**Override the wasCollected function, use Implementation cause it's a blueprint native event*/
	void WasCollected_Implementation() override;

	int GetScoreAmount();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		int ScoreAmount = 5;

private:
	/**index of material element to change*/
	int Body_Element_Index = 0;

};
