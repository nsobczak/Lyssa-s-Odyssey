// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Triggers/CustomTriggerBase.h"
#include "Engine/DataTable.h"
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

	/**dataTable containing dialogues*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerDialogue")
		UDataTable* DialogueDataTable;

	/**list of dialogue rows from dataTable*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerDialogue")
		TArray< FName> DialogueRows;

	/**list of dialogues to display*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TriggerDialogue")
		TArray<FStructDialogue> DialogueToDisplay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TriggerDialogue")
		int CurrentDialogueIndex = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnTriggerDetected_Implementation() override;

	UPROPERTY(VisibleAnywhere, Category = "TriggerDialogue")
		bool IsPlayerActorThatTriggers = true;

	UPROPERTY(VisibleAnywhere, Category = "TriggerDebug")
		class ALyssa* Lyssa;

	UFUNCTION()
		void OnActionAccept();

	UPROPERTY(VisibleAnywhere, Category = "TriggerDebug")
		bool IsEventAccept = false;

private:
	class ALevelGameMode* CurrentGameMode;

	//3 variables string: currentText - toConsumedText - targetText
	//delay, use textSpeed variable for delay
	// currentText += toConsumedText[0] //append
	// toConsumedText = rightChop(toConsumedText, 1)
	// if click: if currentText == targetText => CurrentGameMode->UpdateDialogue(nextDisalogue)
	//			 else: currentText = targetText
};
