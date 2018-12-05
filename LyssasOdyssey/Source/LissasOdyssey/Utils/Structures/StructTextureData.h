// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture.h"
#include "StructTextureData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FStructTextureData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FStructTextureData();

	// The 'Name' column is created by default

	/** Picture to use*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture* Texture;
};
