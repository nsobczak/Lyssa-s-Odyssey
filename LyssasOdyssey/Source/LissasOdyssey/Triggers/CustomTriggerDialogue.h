// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers/CustomTriggerBase.h"
#include "Utils/Structures/StructDialogue.h"
#include "CustomTriggerDialogue.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API ACustomTriggerDialogue : public ACustomTriggerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomTriggerDialogue();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**should Cursor be displayed With Dialogue*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerDialogue")
		bool DisplayCursorWithDialogue = true;

	/**list of dialogue to display*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerDialogue")
		TArray<FStructDialogue> DialogueToDisplay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TriggerDialogue")
		int CurrentDialogueIndex = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnTriggerDetected_Implementation() override;

	UPROPERTY(VisibleAnywhere, Category = "TriggerDialogue")
		bool IsPlayerActorThatTriggers = true;

};
