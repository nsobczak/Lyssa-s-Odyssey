#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Materials/MaterialInterface.h"
#include "Utils/Structures/StructTextTrad.h"
#include "StructMaterialWithName.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FStructMaterialWithName : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FStructMaterialWithName();

	// The 'Name' column is created by default

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStructTextTrad MatName;
};