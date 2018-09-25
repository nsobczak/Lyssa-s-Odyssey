#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Characters/Fylgja/Fylgja.h"
#include "Characters/CharacterBase.h"

#include "Lyssa.generated.h"

UCLASS()
class LISSASODYSSEY_API ALyssa : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALyssa(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Fylgja")
		AFylgja* GetFylgja() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float initialPosZValue;

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

private:
	/** Static mesh to represent the Lyssa in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyssa", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* LyssaMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fylgja", meta = (AllowPrivateAccess = "true"))
		class AFylgja* Fylgja;
};
