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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DialogueDebug")
		TArray<FStructDialogue> DialogueToDisplay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueDebug")
		int CurrentDialogueIndex = 0;

	UFUNCTION()
		void TypewriterEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateDialogueData();

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DialogueDebug", meta = (AllowPrivateAccess = "true"))
		bool DEBUG = false;

	class ALevelGameMode* CurrentGameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueDebug", meta = (AllowPrivateAccess = "true"))
		FString CurrentText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueDebug", meta = (AllowPrivateAccess = "true"))
		FString ToConsumedText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DialogueDebug", meta = (AllowPrivateAccess = "true"))
		FString TargetText;

	float TextTimer = 0;
};
