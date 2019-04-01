// Fill out your copyright notice in the Description page of Project Settings.

#include "Gate.h"
#include "WorldAssets/GateLock.h"

// Sets default values
AGate::AGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGate::BeginPlay()
{
	Super::BeginPlay();

	this->IsOpen = StartOpen;

	//locks
	TArray<AActor*> childActors;
	GetAllChildActors(childActors);

	for (size_t i = 0; i < childActors.Num(); ++i)
	{
		AGateLock* currentLock = Cast<AGateLock>(childActors[i]);
		KeyLockNature currentNature = currentLock->GetLockNature();

		switch (currentNature)
		{
		case BLUE:
			if (useBlueLock)
				Locks.Add(currentLock);
			else
				currentLock->Destroy();
			break;

		case RED:
			if (useRedLock)
				Locks.Add(currentLock);
			else
				currentLock->Destroy();
			break;

		case YELLOW:
			if (useYellowLock)
				Locks.Add(currentLock);
			else
				currentLock->Destroy();
			break;

		case GREEN:
			if (useGreenLock)
				Locks.Add(currentLock);
			else
				currentLock->Destroy();
			break;

		case PURPLE:
			if (usePurpleLock)
				Locks.Add(currentLock);
			else
				currentLock->Destroy();
			break;

		default:
			break;
		}

	}

	IsInitialized = true;
}

bool AGate::GetIsOpen()
{
	return this->IsOpen;
}

void AGate::SetIsOpen(bool newState)
{
	this->IsOpen = newState;
}

void AGate::ChangeIsOpen()
{
	this->IsOpen = !this->IsOpen;
}

TArray<AGateLock*> AGate::GetLocks()
{
	return Locks;
}

void AGate::AddLock(AGateLock* locktoAdd)
{
	Locks.Add(locktoAdd);
}

bool AGate::BreakLock(AGateLock* locktoBreak)
{
	if (Locks.Contains(locktoBreak))
	{
		Locks.Remove(locktoBreak);
		locktoBreak->DelayedDestroy();
		return true;
	}
	else
	{
		return false;
	}
}


// Called every frame
void AGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

