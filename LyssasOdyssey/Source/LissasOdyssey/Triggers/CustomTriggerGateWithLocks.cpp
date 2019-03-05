// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerGateWithLocks.h"
#include "Characters/Lyssa/Lyssa.h"
#include "WorldAssets/Gate.h"
#include "WorldAssets/GateLock.h"

// Sets default values
ACustomTriggerGateWithLocks::ACustomTriggerGateWithLocks()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

void ACustomTriggerGateWithLocks::BeginPlay()
{
	Super::BeginPlay();

	UseConditionToTriggerGates = true;
}

void ACustomTriggerGateWithLocks::OnTriggerDetected_Implementation()
{
	Super::OnTriggerDetected_Implementation();

	ALyssa* lyssa = Cast<ALyssa>(ActorThatTriggers);
	if (lyssa)
	{
		for (size_t i = 0; i < GatesToControl.Num(); ++i)
		{
			AGate* currentGate = GatesToControl[i];
			TArray<AGateLock*> locksToBreak;

			for (size_t j = 0; j < lyssa->GetKeyPickups().Num(); ++j)
			{
				KeyLockNature keyNature = lyssa->GetKeyPickups()[j];
				AGateLock* currentGateLock = nullptr;

				switch (keyNature)
				{
				case BLUE:
					for (size_t k = 0; k < currentGate->GetLocks().Num(); ++k)
					{
						if (currentGate->GetLocks()[k]->GetLockNature() == KeyLockNature::BLUE)
						{
							currentGateLock = currentGate->GetLocks()[k];
							break;
						}
					}
					break;

				case RED:
					for (size_t k = 0; k < currentGate->GetLocks().Num(); ++k)
					{
						if (currentGate->GetLocks()[k]->GetLockNature() == KeyLockNature::RED)
						{
							currentGateLock = currentGate->GetLocks()[k];
							break;
						}
					}
					break;

				case YELLOW:
					for (size_t k = 0; k < currentGate->GetLocks().Num(); ++k)
					{
						if (currentGate->GetLocks()[k]->GetLockNature() == KeyLockNature::YELLOW)
						{
							currentGateLock = currentGate->GetLocks()[k];
							break;
						}
					}
					break;

				case GREEN:
					for (size_t k = 0; k < currentGate->GetLocks().Num(); ++k)
					{
						if (currentGate->GetLocks()[k]->GetLockNature() == KeyLockNature::GREEN)
						{
							currentGateLock = currentGate->GetLocks()[k];
							break;
						}
					}
					break;

				case PURPLE:
					for (size_t k = 0; k < currentGate->GetLocks().Num(); ++k)
					{
						if (currentGate->GetLocks()[k]->GetLockNature() == KeyLockNature::PURPLE)
						{
							currentGateLock = currentGate->GetLocks()[k];
							break;
						}
					}
					break;

				default:
					break;
				}

				if (currentGateLock)
				{
					//break lock + remove key
					currentGate->BreakLock(currentGateLock);
					lyssa->RemoveKeyPickup(keyNature);
				}
			}


			if (currentGate->GetLocks().Num() <= 0)
				currentGate->SetIsOpen(ShouldTriggerOpen);
		}
	}
}