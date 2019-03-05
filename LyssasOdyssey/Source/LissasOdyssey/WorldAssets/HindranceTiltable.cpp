// Fill out your copyright notice in the Description page of Project Settings.

#include "HindranceTiltable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "WorldAssets/Wall.h"
#include "Characters/CharacterActors/Shot.h"


// Sets default values
AHindranceTiltable::AHindranceTiltable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HindranceSKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HindranceSKMesh"));
	RootComponent = (USceneComponent*)HindranceSKMesh;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->AttachToComponent(HindranceSKMesh, FAttachmentTransformRules::KeepRelativeTransform);


	InvisibleWallLeft_ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("InvisibleWallLeft"));
	//InvisibleWallLeft_ChildActorComponent->SetMobility(EComponentMobility::Movable);
	InvisibleWallLeft_ChildActorComponent->AttachToComponent(HindranceSKMesh, FAttachmentTransformRules::KeepRelativeTransform);
	InvisibleWallLeft_ChildActorComponent->SetRelativeLocationAndRotation(WLLocation, WLRotator);
	InvisibleWallLeft_ChildActorComponent->SetRelativeScale3D(WLScale);

	InvisibleWallRight_ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("InvisibleWallRight"));
	//InvisibleWallRight_ChildActorComponent->SetMobility(EComponentMobility::Movable);
	InvisibleWallRight_ChildActorComponent->AttachToComponent(HindranceSKMesh, FAttachmentTransformRules::KeepRelativeTransform);
	InvisibleWallRight_ChildActorComponent->SetRelativeLocationAndRotation(WRLocation, WRRotator);
	InvisibleWallRight_ChildActorComponent->SetRelativeScale3D(WRScale);
}


// Called when the game starts or when spawned
void AHindranceTiltable::BeginPlay()
{
	Super::BeginPlay();

	ArrowMesh->SetVisibility(false);
	HindranceSKMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	InvisibleWallLeft = Cast<AWall>(InvisibleWallLeft_ChildActorComponent->GetChildActor());
	if (InvisibleWallLeft)
	{
		InvisibleWallLeft->GetBaseMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InvisibleWallLeft is null"));
	}

	InvisibleWallRight = Cast<AWall>(InvisibleWallRight_ChildActorComponent->GetChildActor());
	if (InvisibleWallRight)
	{
		InvisibleWallRight->GetBaseMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InvisibleWallRight is null"));
	}
}


void AHindranceTiltable::HandleOverlap_Implementation()
{
	//get overlaping actors and store them in an array
	TArray<AActor*> collectedActors;
	HindranceSKMesh->GetOverlappingActors(collectedActors);

	for (size_t i = 0; i < collectedActors.Num(); ++i)
	{
		AActor* currentActor = collectedActors[i];
		AShot* currentShot = Cast<AShot>(currentActor);
		if (currentShot && currentShot->CanKillFoe)
		{
			IsActivated = true;
			HindranceSKMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;

			if (InvisibleWallLeft && InvisibleWallRight)
			{
				InvisibleWallLeft->GetBaseMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				InvisibleWallRight->GetBaseMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
	}
}


// Called every frame
void AHindranceTiltable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsActivated)
	{
		HandleOverlap_Implementation();
	}
}

