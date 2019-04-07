// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"

#include "GameModes/MainGameMode.h"
#include "Characters/CharacterWithInputs.h"
#include "Characters/Lyssa/Lyssa.h"


int UButtonBase::ButtonCountInActiveGroup;
int UButtonBase::ActiveButtonGroupID;

UButtonBase::UButtonBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UButtonBase::InitializeButtonsTArray()
{
	TArray<UUserWidget*> userWidgetArray;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), userWidgetArray, UButtonBase::StaticClass(), false);

	ButtonsInGroup.Reset(0);
	if (DEBUG)
		UE_LOG(LogTemp, Warning, TEXT("for button widget %s with group id = %i and id = %i, initialize following elements: "),
			*(GetName()), ButtonLinks.ButtonGroupUniqueID, ButtonLinks.ButtonUniqueID);

	for (size_t i = 0; i < userWidgetArray.Num(); ++i)
	{
		UUserWidget* uw = userWidgetArray[i];
		if (uw->IsVisible())
		{
			UButtonBase* bw = Cast<UButtonBase>(uw);
			if (bw->ButtonLinks.ButtonGroupUniqueID == ButtonLinks.ButtonGroupUniqueID)
			{
				ButtonsInGroup.Add(bw);
				if (DEBUG)
					UE_LOG(LogTemp, Log, TEXT("button widget name = %s | group id = %i | id = %i"),
						*(bw->GetName()), bw->ButtonLinks.ButtonGroupUniqueID, bw->ButtonLinks.ButtonUniqueID);
			}
		}
	}
}

UButtonBase* UButtonBase::FindButtonWithID(int id)
{
	if (id > -1)
	{
		for (size_t i = 0; i < ButtonsInGroup.Num(); i++)
		{
			UButtonBase* currentButton = ButtonsInGroup[i];
			if (currentButton->ButtonLinks.ButtonUniqueID == id)
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

	NextButton_Up = FindButtonWithID(ButtonLinks.NextButtonID_Up);
	NextButton_Right = FindButtonWithID(ButtonLinks.NextButtonID_Right);
	NextButton_Bottom = FindButtonWithID(ButtonLinks.NextButtonID_Bottom);
	NextButton_Left = FindButtonWithID(ButtonLinks.NextButtonID_Left);
}


#pragma region Callbacks
void UButtonBase::OnActionAccept()
{
	if (Timer >= TimeBetweenInputs)
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("OnActionAccept"));
		ClickedEffect();
		Timer = 0;
	}
}

void UButtonBase::OnActionReturn()
{
	if (Timer >= TimeBetweenInputs)
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("OnActionReturn"));
		ReturnEffect();
		Timer = 0;
	}
}


void UButtonBase::OnActionMoveUp()
{
	if (Timer >= TimeBetweenInputs && !BlockUpperLink && NextButton_Up && NextButton_Up->IsVisible())
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("OnActionMoveUp: Activated %s | Deactivated %s"), *(NextButton_Up->GetName()), *(this->GetName()));
		Timer = 0;

		NextButton_Up->SetIsActive(true);
		this->SetIsActive(false);
	}
}

void UButtonBase::OnActionMoveRight()
{
	if (Timer >= TimeBetweenInputs && !BlockRightLink && NextButton_Right && NextButton_Right->IsVisible())
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("OnActionMoveRight: Activated %s | Deactivated %s"), *(NextButton_Right->GetName()), *(this->GetName()));
		Timer = 0;

		NextButton_Right->SetIsActive(true);
		this->SetIsActive(false);
	}
}

void UButtonBase::OnActionMoveDown()
{
	if (Timer >= TimeBetweenInputs && !BlockBottomLink && NextButton_Bottom && NextButton_Bottom->IsVisible())
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("OnActionMoveDown: Activated %s | Deactivated %s"), *(NextButton_Bottom->GetName()), *(this->GetName()));
		Timer = 0;

		NextButton_Bottom->SetIsActive(true);
		this->SetIsActive(false);
	}
}

void UButtonBase::OnActionMoveLeft()
{
	if (Timer >= TimeBetweenInputs && !BlockLeftLink && NextButton_Left && NextButton_Left->IsVisible())
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("OnActionMoveLeft: Activated %s | Deactivated %s"), *(NextButton_Left->GetName()), *(this->GetName()));
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
		//WasAlreadyActivated = false;
		OnDeactivation();
	}
	else
	{
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("%s is active"), *(this->GetName()));
		OnActivation();
	}
}

bool UButtonBase::GetIsActive()
{
	return IsActive;
}

void UButtonBase::DeactivateAllButtonsInGroup()
{
	for (size_t i = 0; i < ButtonsInGroup.Num(); ++i)
	{
		UButtonBase* cButton = ButtonsInGroup[i];
		if (cButton->IsActive == true)
		{
			cButton->IsActive = false;
		}
	}
}

void UButtonBase::SetButtonLinks(FStructButtonLinks newButtonLinks)
{
	ButtonLinks = newButtonLinks;
}


//static
void UButtonBase::ResetButtonsInGroup()
{
	if (UButtonBase::ButtonCountInActiveGroup != 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ResetButtonsInGroup called, it were equal to %i"), UButtonBase::ButtonCountInActiveGroup);
		UButtonBase::ButtonCountInActiveGroup = 0;
	}
}


void UButtonBase::NativeConstruct()
{
	if (IsVisible())
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
			//ButtonBase->OnHovered.AddDynamic(this, &UButtonBase::OnActivation);
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


		UButtonBase::ButtonCountInActiveGroup += 1;
		if (DEBUG) UE_LOG(LogTemp, Log, TEXT("ButtonCountInActiveGroup = %i"), UButtonBase::ButtonCountInActiveGroup);
		if (UButtonBase::ActiveButtonGroupID != ButtonLinks.ButtonGroupUniqueID)
		{
			UButtonBase::ActiveButtonGroupID = ButtonLinks.ButtonGroupUniqueID;
		}

		if (ButtonLinks.ButtonUniqueID == 0)
			SetIsActive(true);

		if (DEBUG) UE_LOG(LogTemp, Warning, TEXT("NativeConstruct called for %s"), *(GetName()));
	}
}


void UButtonBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (ButtonsInGroup.Num() < UButtonBase::ActiveButtonGroupID)
	{
		InitializeButtonLinks();
	}

	if (IsActive)
	{
		Timer += InDeltaTime;
	}
}
