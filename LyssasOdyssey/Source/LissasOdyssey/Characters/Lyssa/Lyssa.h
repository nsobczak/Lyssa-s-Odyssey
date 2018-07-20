// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h" 
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "ConstructorHelpers.h"
#include "../CharacterBase.h"
#include "Lyssa.generated.h"

UCLASS()
class LISSASODYSSEY_API ALyssa : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALyssa(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// === Components ===
	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* MainCamera;

	//// === Input ===
	///**handles moving forward/backward*/
	//UFUNCTION()
	//	void MoveForward(float Val);
	///**handles strafing*/
	//UFUNCTION()
	//	void MoveRight(float Val);

	///**sets jump flag when key is pressed*/
	//UFUNCTION()
	//	void OnStartJump();
	///**clears jump flag when key is released*/
	//UFUNCTION()
	//	void OnStopJump();
	
	
};
