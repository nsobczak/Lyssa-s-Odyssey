// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/ActorWithMeshBase.h"
#include "Utils/GameEnums.h"
#include "GateLock.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API AGateLock : public AActorWithMeshBase
{
	GENERATED_BODY()


public:
	AGateLock();

	TEnumAsByte<KeyLockNature> GetLockNature();

protected:
	UPROPERTY(EditAnywhere, Category = "Key", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<KeyLockNature> LockNature = KeyLockNature::YELLOW;
};
