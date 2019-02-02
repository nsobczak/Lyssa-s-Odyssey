// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorWithMeshBase.h"
#include "MinimapCorner.generated.h"

UENUM(BlueprintType)
enum CornerNature { None, TopLeft, TopRight, BottomLeft, BottomRight };

UCLASS()
class LISSASODYSSEY_API AMinimapCorner : public AActorWithMeshBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMinimapCorner();

	UFUNCTION(BlueprintCallable, Category = "Corner")
		CornerNature GetNature();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Corner")
		bool ShouldMeshBeVisible = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Corner")
		TEnumAsByte<CornerNature> Nature = CornerNature::None;
};
