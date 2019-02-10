// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelPortal.h"
#include "Characters/Lyssa/Lyssa.h"
#include "Utils/GameConstants.h"
#include "GameModes/LevelGameMode.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

void ALevelPortal::InitializeText(UTextRenderComponent* textToInit, bool isFrontText)
{
	textToInit->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	textToInit->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	textToInit->TextRenderColor = GameConstants::COLOR_TEXT_LIGHT_GRAY;
	textToInit->SetRelativeScale3D(FVector(10, 10, 10));
	if (isFrontText)
		textToInit->SetRelativeLocationAndRotation(FVector(300.0f, 0, 90.0f), FQuat::MakeFromEuler(FVector(0, 80.0f, 0)));
	else
		textToInit->SetRelativeLocationAndRotation(FVector(-300.0f, 0, 90.0f), FQuat::MakeFromEuler(FVector(0, 100.0f, 0)));
	textToInit->SetText(FText::FromString("Text"));
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

	TextPickup = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextTest"));
	TextPickup->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InitializeText(TextPickup, false);
}

void ALevelPortal::UpdateTexts()
{
	CurrentLGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentLGameMode)
	{
		int pickupCurrentScore = 0, pickupMaxScore = 0;
		FString levelTitle = "";

		switch (LevelToOpen)
		{
		case Canyon:
			UE_LOG(LogTemp, Log, TEXT("Canyon"));
			levelTitle = GameConstants::LVL_TITLE_CANYON;
			pickupCurrentScore = CurrentLGameMode->Lyssa->GetTMapPlayerPickupAmountByLevel().FindRef(LevelToOpen);
			pickupMaxScore = GameConstants::PICKUP_SCORE_MAX_CANYON;
			break;
		case Forest:
			UE_LOG(LogTemp, Log, TEXT("Forest"));
			levelTitle = GameConstants::LVL_TITLE_FOREST;
			pickupCurrentScore = CurrentLGameMode->Lyssa->GetTMapPlayerPickupAmountByLevel().FindRef(LevelToOpen);
			pickupMaxScore = GameConstants::PICKUP_SCORE_MAX_FOREST;
			break;
		case Ice:
			UE_LOG(LogTemp, Log, TEXT("Ice"));
			levelTitle = GameConstants::LVL_TITLE_ICE;
			pickupCurrentScore = CurrentLGameMode->Lyssa->GetTMapPlayerPickupAmountByLevel().FindRef(LevelToOpen);
			pickupMaxScore = GameConstants::PICKUP_SCORE_MAX_ICE;
			break;
		case Volcano:
			UE_LOG(LogTemp, Log, TEXT("Volcano"));
			levelTitle = GameConstants::LVL_TITLE_VOLCANO;
			pickupCurrentScore = CurrentLGameMode->Lyssa->GetTMapPlayerPickupAmountByLevel().FindRef(LevelToOpen);
			pickupMaxScore = GameConstants::PICKUP_SCORE_MAX_VOLCANO;
			break;
		case Playground:
			UE_LOG(LogTemp, Log, TEXT("Playground"));
			levelTitle = GameConstants::LVL_TITLE_PLAYGROUND;
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("Default"));
			levelTitle = GameConstants::LVL_TITLE_HUB;
			break;
		}

		// Assign texts 
		if (TextFront)
			TextFront->SetText(FText::FromString(levelTitle));
		else
			UE_LOG(LogTemp, Error, TEXT("TextFront is null"));

		if (NameOnBothSides) TextPickup->SetText(FText::FromString(levelTitle));
		else
		{
			FString pickupString = FString::FromInt(pickupCurrentScore) + "/" + FString::FromInt(pickupMaxScore);
			FText pickupText = FText::FromString(pickupString);
			UE_LOG(LogTemp, Log, TEXT("pickupString = %s"), *pickupString);
			if (TextPickup)
				TextPickup->SetText(pickupText);
			else
				UE_LOG(LogTemp, Error, TEXT("TextTest is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentLGameMode is null UpdateTexts from ALevelPortal"));
	}
}

void ALevelPortal::UpdateLevelToOpenName()
{
	switch (LevelToOpen)
	{
	case Canyon:
		LevelToOpenName = GameConstants::LVL_MAP_TITLE_CANYON;
		break;
	case Forest:
		LevelToOpenName = GameConstants::LVL_MAP_TITLE_FOREST;
		break;
	case Ice:
		LevelToOpenName = GameConstants::LVL_MAP_TITLE_ICE;
		break;
	case Volcano:
		LevelToOpenName = GameConstants::LVL_MAP_TITLE_VOLCANO;
		break;
	case Playground:
		LevelToOpenName = GameConstants::LVL_MAP_TITLE_PLAYGROUND;
		break;
	default:
		LevelToOpenName = GameConstants::LVL_MAP_TITLE_HUB;
		break;
	}
}

// Called when the game starts or when spawned
void ALevelPortal::BeginPlay()
{
	Super::BeginPlay();

	UpdateTexts();
	UpdateLevelToOpenName();
}

void ALevelPortal::SwapLevel()
{
	UWorld* currentWorld = GetWorld();

	FString CurrentLevel = currentWorld->GetMapName();
	UGameplayStatics::OpenLevel(GetWorld(), LevelToOpenName);
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

