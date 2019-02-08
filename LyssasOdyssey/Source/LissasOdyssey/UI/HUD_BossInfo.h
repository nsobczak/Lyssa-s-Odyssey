// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_BossInfo.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API UHUD_BossInfo : public UUserWidget
{
	GENERATED_BODY()


public:
	UHUD_BossInfo(const FObjectInitializer& ObjectInitializer);

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void BuildWidget();

	UFUNCTION(BlueprintCallable, Category = "HUD_boss")
		class ABoss*  GetCurrentBoss();
	UFUNCTION(BlueprintCallable, Category = "HUD_boss")
		void SetCurrentBoss(class ABoss* newBoss);

	UFUNCTION(BlueprintCallable, Category = "HUD_boss")
		void ShowInfo();
	UFUNCTION(BlueprintCallable, Category = "HUD_boss")
		void HideInfo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD_boss")
		class ABoss* CurrentBoss;

	UPROPERTY(VisibleAnywhere, Category = "HUD_boss")
		class ALevelGameMode* LevelGameMode;
};
