// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SwapLevel();

private:
	/** Static mesh to represent the Lyssa in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PortalMesh;

	UFUNCTION()
		void HandleOverlap();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = "true"))
		FName LevelName = "MainMenu";
};
