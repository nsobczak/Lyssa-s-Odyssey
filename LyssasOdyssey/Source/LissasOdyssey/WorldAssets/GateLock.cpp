// Fill out your copyright notice in the Description page of Project Settings.

#include "GateLock.h"
#include "Engine/Public/TimerManager.h"

// Sets default values for this actor's properties
AGateLock::AGateLock()
{
}

//report the power level of the battery
TEnumAsByte<KeyLockNature> AGateLock::GetLockNature()
{
	return LockNature;
}

void AGateLock::CustomDestroy()
{
	this->Destroy();
}


void AGateLock::DelayedDestroy()
{
	UE_LOG(LogTemp, Log, TEXT("break lock: %s"), *(this->GetName()));
	BreakLockEffect();

	FTimerHandle TimerHandle; // Handle to manage the timer
	FTimerDelegate TimerDel; //Bind function with parameters
	TimerDel.BindUFunction(this, FName("CustomDestroy"));
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, SoundBreakLockDuration, false);
}