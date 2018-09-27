// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameModes/MainGameMode.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Characters/Foes/Foe.h"
#include "WorldAssets/FinishArea.h"

//#include "GameFramework/HUD.h"
//#include "Blueprint/UserWidget.h"

#include "LevelGameMode.generated.h"

//enum to store the current state of gameplay
UENUM(BlueprintType)
enum class ELevelPlayState : uint8
{
	EPlaying,
	EGameOver,
	ELevelCompleted,
	EUnknown
};

UCLASS()
class LISSASODYSSEY_API ALevelGameMode : public AMainGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		AFinishArea* FinishArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		ALyssa* Lyssa;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		TArray<AFoe*> Foes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameController")
		float CollisionDistThreshold = 300.0f;

	UFUNCTION(BlueprintCallable, Category = "GameController")
		void CheckForLevelCompleted();

	UFUNCTION(BlueprintPure, Category = "GameController")
		ELevelPlayState GetCurrentState()const;

	void SetCurrentState(ELevelPlayState newState);

	//TODO: add infos here
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameController")
		float TimeLimit = 99.0f;

	float Timer = 0.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameController")
		float DamageRate = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameController")
		float DamageRateTimer = 0.0f;

	/**widget to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

private:
	void CheckForDeath();

	/**Keeps track of the current playing state*/
	UPROPERTY(VisibleAnywhere, Category = "GameController")
		ELevelPlayState Currentstate;

	/**handle any function call that rely upon changing the playing state of the game*/
	void HandleNewState(ELevelPlayState newState);

};
