// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructButtonLinks.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FStructButtonLinks
{
	GENERATED_USTRUCT_BODY()

public:
	FStructButtonLinks();
	FStructButtonLinks(int32 buttonGroupUniqueID, int32 buttonUniqueID,
		int32 nextButtonID_Up, int32 nextButtonID_Right, int32 nextButtonID_Bottom, int32 nextButtonID_Left);

	void SetButtonLinks(int32 buttonGroupUniqueID, int32 buttonUniqueID,
		int32 nextButtonID_Up, int32 nextButtonID_Right, int32 nextButtonID_Bottom, int32 nextButtonID_Left);

	/** button group ID, -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int32 ButtonGroupUniqueID;
	/** Unique ID in group, 0 means button will be active by default, -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int32 ButtonUniqueID;
	/** -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int32 NextButtonID_Up;
	/** -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int32 NextButtonID_Right;
	/** -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int32 NextButtonID_Bottom;
	/** -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int32 NextButtonID_Left;
};