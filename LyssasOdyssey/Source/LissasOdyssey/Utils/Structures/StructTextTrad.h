// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StructTextTrad.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FStructTextTrad : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FStructTextTrad();

	// The 'Name' column is created by default

	/** english*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString en;

	/** french*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString fr;
};
