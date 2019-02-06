// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomTriggerBase.h"
#include "Engine/TriggerBase.h"
#include "Engine/Public/TimerManager.h"

// Sets default values
ACustomTriggerBase::ACustomTriggerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	TriggerCrossMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	RootComponent = (USceneComponent*)TriggerCrossMesh;
}

// Called when the game starts or when spawned
void ACustomTriggerBase::BeginPlay()
{
	Super::BeginPlay();

	// === Trigger === 
	TArray<AActor*> Comps;
	GetAllChildActors(Comps);
	for (size_t i = 0; i < Comps.Num(); i++)
	{
		if (Comps[i]->IsA(ATriggerBase::StaticClass()))
			TriggerElement = (ATriggerBase*)Comps[i];
	}

	TriggerCrossMesh->SetVisibility(false, false);
}

void ACustomTriggerBase::OnTriggerDetected_Implementation()
{
	//triggered
	IsTriggered = true;
	//UE_LOG(LogTemp, Log, TEXT("%s triggered %s"), *(ActorThatTriggers->GetName()), *(TriggerElement->GetName()));
}

// Called every frame
void ACustomTriggerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr != ActorThatTriggers && nullptr != TriggerElement)
	{
		if (TriggerElement->IsOverlappingActor(ActorThatTriggers))
		{
			if (!IsTriggered && (!hasBeenTriggered || CanBeTriggeredSeveralTimes))
			{
				//delay event
				FTimerHandle TimerHandle; // Handle to manage the timer
				FTimerDelegate TimerDel; //Bind function with parameters
				TimerDel.BindUFunction(this, OnTriggerDetectedName);
				GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, DelayBfrTriggerDetection, false);

				hasBeenTriggered = true;
			}
		}
		else if (IsTriggered)
		{
			IsTriggered = false;
		}
	}
}

