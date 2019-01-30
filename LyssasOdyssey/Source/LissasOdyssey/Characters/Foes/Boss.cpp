// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"

ABoss::ABoss(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	CurrentAttackState = EBossAttackState::ENotAttacking;
}

void ABoss::Tick(float DeltaTime)
{
	CurrentStateTimer += DeltaTime;

	switch (CurrentAttackState)
	{
	case	EBossAttackState::EThrowProjectiles:
		Super::Tick(DeltaTime);

		if (CurrentStateTimer >= Timer_SpawnShots)
		{
			CurrentStateTimer = 0;
			CurrentAttackState = EBossAttackState::EMoveTowards;
		}
		break;

	case EBossAttackState::EMoveTowards:
		if (CurrentStateTimer <= Timer_Move
			|| CurrentStateTimer > Timer_Move + Timer_FastMove && CurrentStateTimer <= 2 * Timer_Move + Timer_FastMove)
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
			CurrentAttackState = EBossAttackState::EArmAttack;
		}
		break;

	case EBossAttackState::EArmAttack:
		if (CurrentStateTimer >= Timer_ArmAttack)
		{
			CurrentStateTimer = 0;
			CurrentAttackState = EBossAttackState::EThrowProjectiles;
		}
		break;

	default:
		//waiting to be activated	
		if (IsFoeActive)
		{
			CurrentAttackState = EBossAttackState::EThrowProjectiles;
		}
		break;
	}
}

bool ABoss::CustomDestroy()
{
	for (size_t i = 0; i < GatesToControl.Num(); i++)
	{
		AGate* currentGate = GatesToControl[i];
		if (currentGate)
			currentGate->SetIsOpen(ShouldOpenGatesAfterDeath);
	}

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
	}
}
