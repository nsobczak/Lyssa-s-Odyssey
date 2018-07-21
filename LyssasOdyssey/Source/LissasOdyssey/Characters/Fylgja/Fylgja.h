// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "Engine.h" 
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"  
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h" //mousePosition, ProjectWorldLocationToScreen
//#include "Runtime/Core/Public/Math/Vector.h"
//#include "ConstructorHelpers.h"
#include "Fylgja.generated.h"

UCLASS()
class LISSASODYSSEY_API AFylgja : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFylgja(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void FollowMousePosition();
	
	
};
