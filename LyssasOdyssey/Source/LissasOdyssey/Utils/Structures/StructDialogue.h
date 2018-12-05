// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Utils/Structures/StructTextureData.h"
#include "Utils/Structures/StructTextTrad.h"
#include "StructDialogue.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FStructDialogue : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FStructDialogue();

	// The 'Name' column is created by default

	/** english*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStructTextureData ProfilePicture;

	/** french*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStructTextTrad DialogueText;
};
