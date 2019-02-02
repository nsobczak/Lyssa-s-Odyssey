// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Minimap.generated.h"

/**
 *
 */
UCLASS()
class LISSASODYSSEY_API UMinimap : public UUserWidget
{
	GENERATED_BODY()

public:
	UMinimap(const FObjectInitializer& ObjectInitializer);

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
		UTexture2D* Map;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
		UTexture2D* PlayerIcon;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
		class AMinimapCorner* CornerTopLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
		class AMinimapCorner* CornerTopRight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
		class AMinimapCorner* CornerBottomLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
		class AMinimapCorner* CornerBottomRight;

	/**Find corners in level and assign them to variable depending on their nature*/
	UFUNCTION(BlueprintCallable, Category = "Map")
		void FindCorners();

private:
	FVector BottomAxis;
	FVector LeftAxis;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
		class ALevelGameMode* LevelGameMode;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
		class ALyssa* LyssaActor;
};
