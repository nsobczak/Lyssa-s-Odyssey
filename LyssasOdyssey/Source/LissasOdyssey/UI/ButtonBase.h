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

	//// Optionally override the tick event
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void BuildWidget();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ButtonBase")
		UButton* ButtonBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ButtonBase")
		UTextBlock* ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonBase")
		bool IsButtonTextVisible = true;

	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
		void ClickedEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "ButtonBase")
		void HoveredEffect();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Debug")
		class AMainGameMode* MainGameMode;
};
