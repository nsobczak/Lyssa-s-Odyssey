// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameModes/MainGameMode.h"

UButtonBase::UButtonBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UButtonBase::BuildWidget()
{
	// Set Up Root Widget
	UCanvasPanel* RootWidget = Cast<UCanvasPanel>(GetRootWidget());
	if (!RootWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("RootWidget is null"));
		//RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Canvas"));
	}
	else
	{

		//add elements

		//ImageMap = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Map"));
		//RootWidget->AddChild(ImageMap);

		TArray<UWidget*> childWidgets;
		WidgetTree->GetChildWidgets(RootWidget, childWidgets);
		for (size_t i = 0; i < childWidgets.Num(); ++i)
		{
			UWidget* currentWidget = childWidgets[i];
			UButton* currentButtonWidget = Cast<UButton>(currentWidget);
			UTextBlock* currentTextBlockWidget = Cast<UTextBlock>(currentWidget);
			if (currentButtonWidget)
				ButtonBase = currentButtonWidget;
			else if (currentTextBlockWidget)
			{
				ButtonText = currentTextBlockWidget;
				ButtonText->SetVisibility(IsButtonTextVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			}
		}

		if (ButtonBase)
		{
			ButtonBase->OnClicked.AddDynamic(this, &UButtonBase::ClickedEffect);
			ButtonBase->OnHovered.AddDynamic(this, &UButtonBase::HoveredEffect);
		}
	}
}

void UButtonBase::NativeConstruct()
{
	// Do some custom setup
	this->MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!MainGameMode) UE_LOG(LogTemp, Error, TEXT("MainGameMode is null"));

	BuildWidget();

	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();
}