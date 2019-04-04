// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"

#include "GameModes/MainGameMode.h"
#include "Characters/CharacterWithInputs.h"
#include "Characters/Lyssa/Lyssa.h"

UButtonBase::UButtonBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UButtonBase::InitializeButtonsTArray()
{
	TArray<UUserWidget*> userWidgetArray;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), userWidgetArray, UButtonBase::StaticClass(), false);

	for (size_t i = 0; i < userWidgetArray.Num(); ++i)
	{
		UUserWidget* w = userWidgetArray[i];
		ButtonsInWidget.Add(Cast<UButtonBase>(w));

		if (ButtonUniqueID == 0)
			UE_LOG(LogTemp, Log, TEXT("widget name = %s"), *(w->GetName()));
	}
}

UButtonBase* UButtonBase::FindButtonWithID(int id)
{
	if (id > -1)
	{
		for (size_t i = 0; i < ButtonsInWidget.Num(); i++)
		{
			UButtonBase* currentButton = ButtonsInWidget[i];
			if (currentButton->ButtonUniqueID == id)
			{
				return currentButton;
			}
		}
	}

	return nullptr;
}

void UButtonBase::InitializeButtonLinks()
{
	InitializeButtonsTArray();

	NextButton_Up = FindButtonWithID(NextButtonID_Up);
	NextButton_Right = FindButtonWithID(NextButtonID_Right);
	NextButton_Bottom = FindButtonWithID(NextButtonID_Bottom);
	NextButton_Left = FindButtonWithID(NextButtonID_Left);
}


#pragma region Callbacks
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
		ReturnEffect();
		Timer = 0;
	}
}

void UButtonBase::OnActionMoveUp()
{
	if (Timer >= TimeBetweenInputs && NextButton_Up)
	{
		//UE_LOG(LogTemp, Log, TEXT("OnActionMoveUp: Activated %s | Deactivated %s"), *(NextButton_Up->GetName()), *(this->GetName()));
		Timer = 0;

		NextButton_Up->SetIsActive(true);
		this->SetIsActive(false);
	}
}

void UButtonBase::OnActionMoveRight()
{
	if (Timer >= TimeBetweenInputs && NextButton_Right)
	{
		//UE_LOG(LogTemp, Log, TEXT("OnActionMoveRight: Activated %s | Deactivated %s"), *(NextButton_Right->GetName()), *(this->GetName()));
		Timer = 0;

		NextButton_Right->SetIsActive(true);
		this->SetIsActive(false);
	}
}

void UButtonBase::OnActionMoveDown()
{
	if (Timer >= TimeBetweenInputs && NextButton_Bottom)
	{
		//UE_LOG(LogTemp, Log, TEXT("OnActionMoveDown: Activated %s | Deactivated %s"), *(NextButton_Bottom->GetName()), *(this->GetName()));
		Timer = 0;

		NextButton_Bottom->SetIsActive(true);
		this->SetIsActive(false);
	}
}

void UButtonBase::OnActionMoveLeft()
{
	if (Timer >= TimeBetweenInputs && NextButton_Left)
	{
		//UE_LOG(LogTemp, Log, TEXT("OnActionMoveLeft: Activated %s | Deactivated %s"), *(NextButton_Left->GetName()), *(this->GetName()));
		Timer = 0;

		NextButton_Left->SetIsActive(true);
		this->SetIsActive(false);
	}
}
#pragma endregion



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
		//assign properties
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


void UButtonBase::SetIsActive(bool isActive)
{
	IsActive = isActive;

	if (isActive == false)
	{
		WasAlreadyActivated = false;
		OnDeactivation();
	}
	else
	{
		OnActivation();
	}
}

bool UButtonBase::GetIsActive()
{
	return IsActive;
}


void UButtonBase::NativeConstruct()
{
	// Do some custom setup
	this->MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!MainGameMode) UE_LOG(LogTemp, Error, TEXT("MainGameMode is null"));

	BuildWidget();

	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();


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


	InitializeButtonLinks();

	if (ButtonUniqueID == 0)
		SetIsActive(true);

	UE_LOG(LogTemp, Warning, TEXT("NativeConstruct called"));
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
	}
}
