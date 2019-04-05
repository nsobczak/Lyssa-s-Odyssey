// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/UMG.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ButtonBase")
		UButton* ButtonBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ButtonBase")
		UTextBlock* ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonBase")
		bool IsButtonTextVisible = true;

	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
		void ClickedEffect();

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

	/** button group ID, -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int ButtonGroupUniqueID = -1;
	/** Unique ID in group, 0 means button will be active by default, -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int ButtonUniqueID = -1;
	/** -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int NextButtonID_Up = -1;
	/** -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int NextButtonID_Right = -1;
	/** -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int NextButtonID_Bottom = -1;
	/** -1 will be ignored*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		int NextButtonID_Left = -1;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Debug")
		class AMainGameMode* MainGameMode;

	static int ButtonCountInActiveGroup;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonBase", meta = (AllowPrivateAccess = "true"))
		bool DEBUG = false;

	TArray<UButtonBase*> ButtonsInWidget;
	void InitializeButtonsTArray();
	void InitializeButtonLinks();

	float TimeBetweenInputs = 0.15f;
	float Timer = 0;

	UButtonBase* NextButton_Up;
	UButtonBase* NextButton_Right;
	UButtonBase* NextButton_Bottom;
	UButtonBase* NextButton_Left;
};
