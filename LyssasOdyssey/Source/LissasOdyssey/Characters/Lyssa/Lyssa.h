#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
//#include "../Fylgja/Fylgja.h"

#include "Engine.h" 
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "ConstructorHelpers.h"

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

	// === Input ===
	/**handles moving forward/backward*/
	UFUNCTION()
		void MoveUp(float Val);
	/**handles strafing*/
	UFUNCTION()
		void MoveRight(float Val);

};
