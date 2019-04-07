// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/UMG.h"
#include "Utils/Structures/StructButtonLinks.h"
#include "ButtonBase.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API UButtonBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UButtonBase(const FObjectInitializer& ObjectInitializer);

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void BuildWidget();

	static void ResetButtonsInGroup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ButtonBase", AdvancedDisplay)
		UButton* ButtonBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ButtonBase", AdvancedDisplay)
		UTextBlock* ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonBase")
		bool IsButtonTextVisible = true;

	/** event called when ActionAccept button is pressed*/
	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
		void ClickedEffect();

	/** event called when ActionReturn button is pressed*/
	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
		void ReturnEffect();

	//UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
	//	void HoveredEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
		void OnActivation();
	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
		void OnDeactivation();

	UFUNCTION(BlueprintCallable, Category = "ButtonBase")
		void SetIsActive(bool isActive);
	UFUNCTION(BlueprintPure, Category = "ButtonBase")
		bool GetIsActive();

	UFUNCTION(BlueprintCallable, Category = "ButtonBase")
		void DeactivateAllButtonsInGroup();

	/** button links*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		FStructButtonLinks ButtonLinks;

	UFUNCTION(BlueprintCallable, Category = "ButtonNavigation")
		void SetButtonLinks(FStructButtonLinks newButtonLinks);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonNavigation", AdvancedDisplay)
		bool BlockUpperLink = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonNavigation", AdvancedDisplay)
		bool BlockRightLink = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonNavigation", AdvancedDisplay)
		bool BlockBottomLink = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonNavigation", AdvancedDisplay)
		bool BlockLeftLink = false;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Debug")
		class AMainGameMode* MainGameMode;

	/** all buttons in current group*/
	static int ButtonCountInActiveGroup;
	/** the currently active group*/
	static int ActiveButtonGroupID;

	UPROPERTY(VisibleAnywhere, Category = "ButtonNavigation")
		bool IsActive = false;

	UFUNCTION(BlueprintCallable, Category = "ButtonBase")
		UButtonBase* FindButtonWithID(int id);

	UFUNCTION()
		void OnActionAccept();
	UFUNCTION()
		void OnActionReturn();
	UFUNCTION()
		void OnActionMoveUp();
	UFUNCTION()
		void OnActionMoveRight();
	UFUNCTION()
		void OnActionMoveDown();
	UFUNCTION()
		void OnActionMoveLeft();


private:
	/** will write logs if true*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonBase", meta = (AllowPrivateAccess = "true"))
		bool DEBUG = false;

	TArray<UButtonBase*> ButtonsInGroup;
	void InitializeButtonsTArray();
	void InitializeButtonLinks();

	float TimeBetweenInputs = 0.15f;
	float Timer = 0;

	UButtonBase* NextButton_Up;
	UButtonBase* NextButton_Right;
	UButtonBase* NextButton_Bottom;
	UButtonBase* NextButton_Left;
};
