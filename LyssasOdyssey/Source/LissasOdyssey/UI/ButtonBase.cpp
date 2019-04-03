// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameModes/MainGameMode.h"
#include "Characters/CharacterWithInputs.h"
#include "Characters/Lyssa/Lyssa.h"

UButtonBase::UButtonBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UButtonBase::OnActionAccept()
{
	if (Timer >= TimeBetweenInputs)
	{
		UE_LOG(LogTemp, Log, TEXT("OnActionAccept"));
		ClickedEffect();
		Timer = 0;
	}
}

void UButtonBase::OnActionReturn()
{
	if (Timer >= TimeBetweenInputs)
	{
		UE_LOG(LogTemp, Log, TEXT("OnActionReturn"));
		Timer = 0;
	}
}

void UButtonBase::OnActionMoveUp()
{
	if (Timer >= TimeBetweenInputs && NextButton_Up)
	{
		UE_LOG(LogTemp, Log, TEXT("OnActionMoveUp"));
		NextButton_Up->SetIsActive(true);
		IsActive = false;
		Timer = 0;
	}
}

void UButtonBase::OnActionMoveRight()
{
	if (Timer >= TimeBetweenInputs && NextButton_Right)
	{
		UE_LOG(LogTemp, Log, TEXT("OnActionMoveRight"));
		NextButton_Right->SetIsActive(true);
		IsActive = false;
		Timer = 0;
	}
}

void UButtonBase::OnActionMoveDown()
{
	if (Timer >= TimeBetweenInputs && NextButton_Bottom)
	{
		UE_LOG(LogTemp, Log, TEXT("OnActionMoveDown"));
		NextButton_Bottom->SetIsActive(true);
		IsActive = false;
		Timer = 0;
	}
}

void UButtonBase::OnActionMoveLeft()
{
	if (Timer >= TimeBetweenInputs && NextButton_Left)
	{
		UE_LOG(LogTemp, Log, TEXT("OnActionMoveLeft"));
		NextButton_Left->SetIsActive(true);
		IsActive = false;
		Timer = 0;
	}
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



	}
}

void UButtonBase::NativeConstruct()
{
	// Do some custom setup
	this->MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!MainGameMode) UE_LOG(LogTemp, Error, TEXT("MainGameMode is null"));

	BuildWidget();


	if (ButtonBase)
	{
		ButtonBase->OnClicked.AddDynamic(this, &UButtonBase::ClickedEffect);
		//ButtonBase->OnHovered.AddDynamic(this, &UButtonBase::HoveredEffect);
		ButtonBase->OnHovered.AddDynamic(this, &UButtonBase::OnActivation);
	}

	//delegates
	ALyssa* lyssa = Cast<ALyssa>(UGameplayStatics::GetPlayerPawn(this, 0));
	ACharacterWithInputs* defCharacter = Cast<ACharacterWithInputs>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (lyssa)
	{
		lyssa->OnAcceptDelegate.AddDynamic(this, &UButtonBase::OnActionAccept);
		lyssa->OnReturnDelegate.AddDynamic(this, &UButtonBase::OnActionReturn);
		lyssa->OnUpDelegate.AddDynamic(this, &UButtonBase::OnActionMoveUp);
		lyssa->OnRightDelegate.AddDynamic(this, &UButtonBase::OnActionMoveRight);
		lyssa->OnDownDelegate.AddDynamic(this, &UButtonBase::OnActionMoveDown);
		lyssa->OnLeftDelegate.AddDynamic(this, &UButtonBase::OnActionMoveLeft);
	}

	else if (defCharacter)
	{
		defCharacter->OnAcceptDelegate.AddDynamic(this, &UButtonBase::OnActionAccept);
		defCharacter->OnReturnDelegate.AddDynamic(this, &UButtonBase::OnActionReturn);
		defCharacter->OnUpDelegate.AddDynamic(this, &UButtonBase::OnActionMoveUp);
		defCharacter->OnRightDelegate.AddDynamic(this, &UButtonBase::OnActionMoveRight);
		defCharacter->OnDownDelegate.AddDynamic(this, &UButtonBase::OnActionMoveDown);
		defCharacter->OnLeftDelegate.AddDynamic(this, &UButtonBase::OnActionMoveLeft);
	}


	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();
}


void UButtonBase::SetIsActive(bool isActive)
{
	IsActive = isActive;
}

bool UButtonBase::GetIsActive()
{
	return IsActive;
}

void UButtonBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (IsActive)
	{
		Timer += InDeltaTime;
		if (!WasAlreadyActivated)
		{
			OnActivation();
			WasAlreadyActivated = true;
			UE_LOG(LogTemp, Log, TEXT("%s is active"), *(this->GetName()));
		}

		//check inputs and deactivate IsActive + WasAlreadyActivated if we move to another one

	}
}
