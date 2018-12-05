// Fill out your copyright notice in the Description page of Project Settings.

#include "MainSaveGame.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"

UMainSaveGame::UMainSaveGame()
{
	//general
	CurrentLanguage = ELanguages::en;

	//graphic
	GraphicalIndex = 2;
	PPIndex = 2;
	AAIndex = 2;
	ShadowIndex = 2;
	FPSIndex = 2;
	ResolutionIndex = 2;

	//audio
	MasterVolumeSliderValue = 1;

	//key
	PlayerKeys = { EKeys::W, EKeys::S, EKeys::A, EKeys::D, EKeys::E, EKeys::Tab };

	//UE_LOG(LogTemp, Log, TEXT("constructed UMainSaveGame"));
}