#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Materials/MaterialInterface.h"
#include "Utils/Structures/StructMaterialWithName.h"
#include "StructLyssaMaterialPreset.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FStructLyssaMaterialPreset : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FStructLyssaMaterialPreset();

	// The 'Name' column is created by default

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStructMaterialWithName Mat1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStructMaterialWithName Mat2;
};