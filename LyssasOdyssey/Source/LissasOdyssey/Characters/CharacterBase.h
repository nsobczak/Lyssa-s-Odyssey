#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class LISSASODYSSEY_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Character")
		float GetCurrentLife();

	/**	function to update the foe's life
	* @param powerChange This is the amount to change the power by, can be positive or negative
	*/
	UFUNCTION(BlueprintCallable, Category = "Character")
		void UpdateLife(float lifeChange);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float MaxLife = 100.0f;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Character")
		float Life;


};
