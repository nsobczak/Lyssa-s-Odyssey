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

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
		float GetCurrentLife();

	/**	function to update the foe's life
	* @param powerChange This is the amount to change the power by, can be positive or negative
	*/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void UpdateLife(float lifeChange);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
		float MaxLife = 100.0f;

	//BlueprintImplementableEvent = we don't need to define function in code at all, just call it and let the blueprint define it
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")
		void DamageEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")
		void DeathEffect();

protected:
	UPROPERTY(VisibleAnywhere, Category = "CharacterBase")
		float Life;


};
