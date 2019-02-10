// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Utils/GameEnums.h"
#include "LevelPortal.generated.h"

UCLASS()
class LISSASODYSSEY_API ALevelPortal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelPortal();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelPortalDebug")
		UTextRenderComponent* TextFront;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelPortalDebug")
		UTextRenderComponent* TextPickup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
		TEnumAsByte<LevelLabels> LevelToOpen;

	UFUNCTION(BlueprintCallable, Category = "Level")
		void UpdateTexts();
	UFUNCTION(BlueprintCallable, Category = "Level")
		void UpdateLevelToOpenName();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelPortalDebug")
		FName LevelToOpenName = "MainMenu";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelPortal")
		class ALevelGameMode* CurrentLGameMode;

	UFUNCTION()
	void SwapLevel();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelPortal")
		bool NameOnBothSides = false;

private:
	/** Static mesh to represent the Mesh in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PortalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PortalInterior;

	UFUNCTION()
		void InitializeText(UTextRenderComponent* textToInit, bool isFrontText = true);

	UFUNCTION()
		void HandleOverlap();

};
