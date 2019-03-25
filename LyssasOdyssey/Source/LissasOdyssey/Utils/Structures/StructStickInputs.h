// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructStickInputs.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FStructStickInputs
{
	GENERATED_USTRUCT_BODY()

public:
	FStructStickInputs();
	FStructStickInputs(float top, float right, float down, float left);

	// 4 values
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> Inputs;
};

