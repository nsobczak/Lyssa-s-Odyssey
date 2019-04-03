#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "CharacterWithInputs.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDefaultAccept);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDefaultReturn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDefaultUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDefaultRight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDefaultDown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionDefaultLeft);

UCLASS()
class LISSASODYSSEY_API ACharacterWithInputs : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ACharacterWithInputs(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Delegates

	UPROPERTY(BlueprintAssignable, Category = "ActionDefaultEvent")
		FActionDefaultAccept OnAcceptDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionDefaultEvent")
		FActionDefaultReturn OnReturnDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionDefaultEvent")
		FActionDefaultUp OnUpDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionDefaultEvent")
		FActionDefaultRight OnRightDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionDefaultEvent")
		FActionDefaultDown OnDownDelegate;
	UPROPERTY(BlueprintAssignable, Category = "ActionDefaultEvent")
		FActionDefaultLeft OnLeftDelegate;

#pragma endregion

	// === Components ===
	//// camera
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	//	UCameraComponent* MainCamera;


	// === Input ===
	/** Wait for level game mode begin function completion and assign keys*/
	UFUNCTION(BlueprintCallable, Category = "Input")
		void WaitForLoadCompletionAndAssignKeys(class AMainGameMode* currentGameMode, class UInputComponent* playerInputComponent, float checkInterval = 0.1f);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**handles moving up*/
	UFUNCTION()
		virtual void MoveUp(float value);
	/**handles moving down*/
	UFUNCTION()
		virtual void MoveDown(float value);
	/**handles left*/
	UFUNCTION()
		virtual void MoveLeft(float value);
	/**handles right*/
	UFUNCTION()
		virtual void MoveRight(float value);

	/**accept button*/
	UFUNCTION()
		virtual void ActionAccept();
	/**return button*/
	UFUNCTION()
		virtual void ActionReturn();

};
