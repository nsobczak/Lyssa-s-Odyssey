// Fill out your copyright notice in the Description page of Project Settings.

#include "GateLock.h"

// Sets default values for this actor's properties
AGateLock::AGateLock()
{
}

//report the power level of the battery
TEnumAsByte<KeyLockNature> AGateLock::GetLockNature()
{
	return LockNature;
}