#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utils/GameEnums.h"

#include "Characters/Fylgja/Fylgja.h"
#include "Characters/CharacterBase.h"

#include "Lyssa.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionAccept);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionReturn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionRight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionLeft);

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

#pragma region Delegates

	UPROPERTY(BlueprintAssignable, Category = "ActionEvent")
		FActionAccept OnAcceptDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionEvent")
		FActionReturn OnReturnDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionEvent")
		FActionUp OnUpDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionEvent")
		FActionRight OnRightDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionEvent")
		FActionDown OnDownDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionEvent")
		FActionLeft OnLeftDelegate;

#pragma endregion

	// === Components ===
	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* MainCamera;


	// === Input ===
	/** Wait for level game mode begin function completion and assign keys*/
	UFUNCTION(BlueprintCallable, Category = "Input")
		void WaitForLoadCompletionAndAssignKeys(class ALevelGameMode* currentGameMode, class UInputComponent* playerInputComponent, float checkInterval = 0.1f);

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

	UFUNCTION(BlueprintCallable, Category = "Lyssa")
		void UpdateRotation();

	/**handles fylgja moving up*/
	UFUNCTION()
		void MoveFUp(float value);
	/**handles fylgja moving down*/
	UFUNCTION()
		void MoveFDown(float value);
	/**handles fylgja left*/
	UFUNCTION()
		void MoveFLeft(float value);
	/**handles fylgja right*/
	UFUNCTION()
		void MoveFRight(float value);

	/**accept button*/
	UFUNCTION()
		void ActionAccept();
	/**return button*/
	UFUNCTION()
		void ActionReturn();

	/**call pause menu*/
	UFUNCTION()
		void PauseGame();

	// === KeyPickup ===
	UFUNCTION(BlueprintPure, Category = "Pickups")
		TArray<TEnumAsByte<KeyLockNature>> GetKeyPickups();

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void AddKeyPickup(TEnumAsByte<KeyLockNature> pickupNatureToAdd);

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		bool RemoveKeyPickup(TEnumAsByte<KeyLockNature> pickupNatureToRemove);

	// === ScorePickupAmount ===
	UFUNCTION(BlueprintPure, Category = "Pickups")
		int GetCurrentScorePickupAmount();

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void UpdateScorePickupAmount(int amountChange);

	UFUNCTION(BlueprintPure, Category = "Pickups")
		TMap<TEnumAsByte<LevelLabels>, int> GetTMapPlayerPickupAmountByLevel();
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void SetTMapPlayerPickupAmountByLevel(TMap<TEnumAsByte<LevelLabels>, int> newTMap);
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		int GetPlayerPickupTotalScore();

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void UpdateTMapPlayerPickupAmountByLevel(LevelLabels LevelToChange, int amountChange);
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void EmplaceTMapPlayerPickupAmountByLevel(LevelLabels LevelToChange, int newAmount);

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void ResetTMapPlayerPickupAmountByLevel();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickups")
		bool IsInitialized_TMapPlayerPickupByLevel = false;


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
		TArray<TEnumAsByte<KeyLockNature>> KeyPickups;

	UPROPERTY(VisibleAnywhere, Category = "Pickups")
		TMap<TEnumAsByte<LevelLabels>, int>TMapPlayerPickupAmountByLevel;

	UPROPERTY(VisibleAnywhere, Category = "Pickups")
		int ScorePickupAmount;

	float leftKeyValue = 0, topKeyValue = 0, rightKeyValue = 0, downKeyValue = 0;
	float rotationAngle = 0;
};
