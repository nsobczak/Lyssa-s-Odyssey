// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelPortal.h"
#include "Characters/Lyssa/Lyssa.h"

// Sets default values
void ALevelPortal::InitializeText(UTextRenderComponent* textToInit, bool isFrontText)
{
	textToInit->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	textToInit->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	textToInit->TextRenderColor = FColor::FromHex("#606060FF");
	textToInit->SetRelativeScale3D(FVector(10, 10, 10));
	if (isFrontText)
		textToInit->SetRelativeLocationAndRotation(FVector(200.0f, 0, 90.0f), FQuat::MakeFromEuler(FVector(0, 80.0f, 0)));
	else
		textToInit->SetRelativeLocationAndRotation(FVector(-200.0f, 0, 90.0f), FQuat::MakeFromEuler(FVector(360.0f, 100.0f, 0)));
	textToInit->SetText(ActualText);
}


ALevelPortal::ALevelPortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the static mesh component
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	RootComponent = (USceneComponent*)PortalMesh;

	PortalInterior = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalInterior"));
	PortalInterior->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TextFront = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextFront"));
	TextFront->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InitializeText(TextFront, true);

	if (TextOnBothSides)
	{
		TextBack = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextBack"));
		TextBack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		InitializeText(TextBack, false);
	}
}

// Called when the game starts or when spawned
void ALevelPortal::BeginPlay()
{
	Super::BeginPlay();

	TextFront->SetText(ActualText);
	if (TextOnBothSides) TextBack->SetText(ActualText);
}

void ALevelPortal::SwapLevel()
{
	UWorld* currentWorld = GetWorld();

	FString CurrentLevel = currentWorld->GetMapName();
	UGameplayStatics::OpenLevel(GetWorld(), LevelToOpen);
}

void ALevelPortal::HandleOverlap()
{
	//get overlaping actors and store them in an array
	TArray<AActor*> collectedActors;
	PortalInterior->GetOverlappingActors(collectedActors);

	for (size_t i = 0; i < collectedActors.Num(); ++i)
	{
		AActor* currentActor = collectedActors[i];
		ALyssa* currentLyssa = Cast<ALyssa>(currentActor);
		if (currentLyssa)
		{
			//go to level
			SwapLevel();
		}
	}
}

// Called every frame
void ALevelPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleOverlap();
}

