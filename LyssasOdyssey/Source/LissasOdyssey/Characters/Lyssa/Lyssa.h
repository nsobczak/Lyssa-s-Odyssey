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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// === Components ===
	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* MainCamera;

	// === Input ===
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**handles moving up*/
	UFUNCTION()
		void MoveUp(float value);
	/**handles moving down*/
	UFUNCTION()
		void MoveDown(float value);
	/**handles left*/
	UFUNCTION()
		void MoveLeft(float value);
	/**handles right*/
	UFUNCTION()
		void MoveRight(float value);

	/**call pause menu*/
	UFUNCTION()
		void PauseGame();

	// === ScorePickupAmount ===
	UFUNCTION(BlueprintPure, Category = "Pickups")
		int GetCurrentScorePickupAmount();

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void UpdateScorePickupAmount(int amountChange);

protected:
	float initialPosZValue;

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void CollectPickups();

private:
	/** Static mesh to represent the Lyssa in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyssa", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* LyssaMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fylgja", meta = (AllowPrivateAccess = "true"))
		class AFylgja* Fylgja;

	UPROPERTY(VisibleAnywhere, Category = "Pickups")
		int ScorePickupAmount;

};
