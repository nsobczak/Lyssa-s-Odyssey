// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Foes/Foe.h"
#include "WorldAssets/Gate.h"
#include "Boss.generated.h"

//enum to store the current state of character
UENUM(BlueprintType)
enum class EBossAttackState : uint8
{
	EThrowProjectiles,
	EMoveTowards,
	EArmAttackL,
	EArmAttackR,
	ENotAttacking
};


UCLASS()
class LISSASODYSSEY_API ABoss : public AFoe
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoss(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gate")
		TArray<AGate*> GatesToControl;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gate")
		bool ShouldOpenGatesAfterDeath = true;

	UFUNCTION(BlueprintPure, Category = "Boss")
		EBossAttackState GetCurrentAttackState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
		FString BossName = "BossName";

protected:
	virtual bool CustomDestroy() override;

	void HandleAttackState(float DeltaTime, bool isUnderPlayerDetectionDistance);

	//BlueprintImplementableEvent = we don't need to define function in code at all, just call it and let the blueprint define it
	UFUNCTION(BlueprintCallable, Category = "Boss")
		void MoveTowardsPlayer(float deltaTime, float speed);

	UPROPERTY(EditAnywhere, Category = "Boss")
		float MoveSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Boss")
		float FastMoveSpeed = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Boss")
		bool SwitchPlayingAudioComponentOnDeath = true;

	UPROPERTY(EditAnywhere, Category = "Boss", meta = (EditCondition = "SwitchPlayingAudioComponentOnDeath"))
		bool SwitchToOtherTheme = false;


private:
	UPROPERTY(VisibleAnywhere, Category = "Boss")
		EBossAttackState CurrentAttackState;

	float CurrentStateTimer = 0;

	UPROPERTY(EditAnywhere, Category = "Boss")
		float Timer_SpawnShots = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Boss")
		float Timer_ArmAttack = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Boss")
		float Timer_Move = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Boss")
		float Timer_FastMove = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = "Boss")
		EBossAttackState LastArmAttack = EBossAttackState::EArmAttackL;
};
