// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
		FName LevelToOpen = "MainMenu";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
		UTextRenderComponent* TextFront;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
		UTextRenderComponent* TextBack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
		FText ActualText = FText::FromString("LevelName");

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SwapLevel();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
		bool TextOnBothSides = true;

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
