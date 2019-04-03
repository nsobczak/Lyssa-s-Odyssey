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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ButtonBase")
		UButton* ButtonBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ButtonBase")
		UTextBlock* ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonBase")
		bool IsButtonTextVisible = true;

	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
		void ClickedEffect();

	//UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
	//	void HoveredEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
		void OnActivation();

	UFUNCTION(BlueprintCallable, Category = "ButtonBase")
		void SetIsActive(bool isActive);
	UFUNCTION(BlueprintPure, Category = "ButtonBase")
		bool GetIsActive();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		UButtonBase* NextButton_Up;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		UButtonBase* NextButton_Right;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		UButtonBase* NextButton_Bottom;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ButtonNavigation")
		UButtonBase* NextButton_Left;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Debug")
		class AMainGameMode* MainGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonNavigation")
		bool IsActive = false;

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
	bool WasAlreadyActivated = false;

	float TimeBetweenInputs = 0.1f;
	float Timer = 0;
};
