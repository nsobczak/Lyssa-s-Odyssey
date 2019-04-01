// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"
#include "GameModes/LevelGameMode.h"
#include "UI/HUD_BossInfo.h"


ABoss::ABoss(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	CurrentAttackState = EBossAttackState::ENotAttacking;
}

EBossAttackState ABoss::GetCurrentAttackState()
{
	return this->CurrentAttackState;
}


void ABoss::HandleAttackState(float DeltaTime, bool isUnderPlayerDetectionDistance)
{
	switch (CurrentAttackState)
	{
	case EBossAttackState::EThrowProjectiles:
		MainFoe(DeltaTime, isUnderPlayerDetectionDistance);

		if (CurrentStateTimer >= Timer_SpawnShots)
		{
			CurrentStateTimer = 0;
			CurrentAttackState = EBossAttackState::EMoveTowards;
		}
		break;

	case EBossAttackState::EMoveTowards:
		if (CurrentStateTimer <= Timer_Move
			|| (CurrentStateTimer > Timer_Move + Timer_FastMove && CurrentStateTimer <= 2 * Timer_Move + Timer_FastMove))
		{
			MoveTowardsPlayer(DeltaTime, MoveSpeed);
		}
		else if (CurrentStateTimer > Timer_Move &&  CurrentStateTimer <= Timer_Move + Timer_FastMove)
		{
			MoveTowardsPlayer(DeltaTime, FastMoveSpeed);
		}
		else
		{
			CurrentStateTimer = 0;
			CurrentAttackState = (LastArmAttack == EBossAttackState::EArmAttackL) ? EBossAttackState::EArmAttackR : EBossAttackState::EArmAttackL;
			LastArmAttack = CurrentAttackState;
		}
		break;

	case EBossAttackState::EArmAttackL:
	case EBossAttackState::EArmAttackR:
		if (CurrentStateTimer >= Timer_ArmAttack)
		{
			CurrentStateTimer = 0;
			CurrentAttackState = EBossAttackState::EThrowProjectiles;
		}
		break;

	default:
		CurrentAttackState = IsFoeActive ? EBossAttackState::EThrowProjectiles : EBossAttackState::ENotAttacking;
		break;
	}
}

void ABoss::Tick(float DeltaTime)
{
	if (!IsFoeActive)
	{
		CurrentAttackState = EBossAttackState::ENotAttacking;
		return;
	}

	CurrentStateTimer += DeltaTime;

	bool isUnderPlayerDetectionDistance = true;
	if (UsePlayerDetectionDistance)
		isUnderPlayerDetectionDistance = FVector::DistSquared(GetActorLocation(), Lyssa->GetActorLocation()) < PlayerDetectionDistance;

	OverlapTimer += DeltaTime;
	if (isUnderPlayerDetectionDistance && OverlapTimer > PlayerOverlapCheckRate)
		CheckForPlayerOverlap(DeltaTime);

	HandleAttackState(DeltaTime, isUnderPlayerDetectionDistance);
}

bool ABoss::CustomDestroy()
{
	for (size_t i = 0; i < GatesToControl.Num(); i++)
	{
		AGate* currentGate = GatesToControl[i];
		if (currentGate)
			currentGate->SetIsOpen(ShouldOpenGatesAfterDeath);
	}

	if (CurrentLGameMode && CurrentLGameMode->HudBossInfo)
	{
		CurrentLGameMode->HudBossInfo->HideInfo();
		CurrentLGameMode->CurrentBoss = nullptr;
		CurrentLGameMode->IsBossActive = false;
	}

	UE_LOG(LogTemp, Log, TEXT("boss destroyed"));

	return Super::CustomDestroy();
}

void ABoss::MoveTowardsPlayer(float deltaTime, float speed)
{
	if (nullptr != Lyssa)
	{
		FVector targetDirection = (Lyssa->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
		targetDirection -= FVector(0, 0, targetDirection.Z); //keep it moving in 2D plan

		FVector newLocation = this->GetActorLocation() + targetDirection * deltaTime * speed;
		SetActorLocation(newLocation);

		if (ShouldLookAtPlayer)
			LookAtPlayer();
	}
}
