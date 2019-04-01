// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD_BossInfo.h"
#include "UMG/Public/UMG.h"
#include "GameModes/LevelGameMode.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

UHUD_BossInfo::UHUD_BossInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UHUD_BossInfo::BuildWidget()
{
	// Set Up Root Widget
	UCanvasPanel* RootWidget = Cast<UCanvasPanel>(GetRootWidget());
	if (!RootWidget)
		UE_LOG(LogTemp, Error, TEXT("RootWidget is null"));

	this->LevelGameMode->HudBossInfo = this;

	this->SetVisibility(ESlateVisibility::Hidden);
	//add elements here
}

void UHUD_BossInfo::NativeConstruct()
{
	// Do some custom setup
	this->LevelGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!LevelGameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("LevelGameMode is null"));
	}
	else
	{
		BuildWidget();

		// Call the Blueprint "Event Construct" node
		Super::NativeConstruct();
	}
}

void UHUD_BossInfo::ShowInfo()
{
	if (this->LevelGameMode->CurrentBoss)
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHUD_BossInfo::HideInfo()
{
	if (this->LevelGameMode->CurrentBoss)
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}

//void UHUD_BossInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//
//}