// Fill out your copyright notice in the Description page of Project Settings.

#include "Minimap.h"
#include "Minimap/MinimapCorner.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameModes/LevelGameMode.h"
#include "Characters/Lyssa/Lyssa.h"
#include "UMG/Public/UMG.h"


UMinimap::UMinimap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMinimap::FindCorners()
{
	TArray<AActor*> corners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinimapCorner::StaticClass(), corners);

	for (size_t i = 0; i < corners.Num(); i++)
	{
		AMinimapCorner* corner = Cast<AMinimapCorner>(corners[i]);
		if (corner)
		{
			CornerNature cornerNature = corner->GetNature();
			switch (cornerNature)
			{
			case TopLeft:
				CornerTopLeft = corner;
				break;
			case TopRight:
				CornerTopRight = corner;
				break;
			case BottomLeft:
				CornerBottomLeft = corner;
				break;
			case BottomRight:
				CornerBottomRight = corner;
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("map corner nature is wrong"));
				break;
			}
		}
		else
			UE_LOG(LogTemp, Error, TEXT("a map corner is null"));
	}

	if (corners.Num() != 4)
		UE_LOG(LogTemp, Error, TEXT("map corner number is wrong"));
}

void UMinimap::BuildWidget()
{
	// Set Up Root Widget
	UCanvasPanel* RootWidget = Cast<UCanvasPanel>(GetRootWidget());
	if (!RootWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("RootWidget is null"));
		//RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Canvas"));
	}

	//add elements

	ImageMap = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Map"));
	RootWidget->AddChild(ImageMap);

	ImagePlayerIcon = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("PlayerIcon"));
	RootWidget->AddChild(ImagePlayerIcon);
}

void UMinimap::NativeConstruct()
{
	// Do some custom setup
	BuildWidget();

	FindCorners();

	this->LevelGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!LevelGameMode) UE_LOG(LogTemp, Error, TEXT("LevelGameMode is null"));
	this->LyssaActor = LevelGameMode->Lyssa;

	if (LevelGameMode->Map)
	{
		ImageMap->SetBrushFromTexture(LevelGameMode->Map, true);
		ImageMSlot = Cast<UCanvasPanelSlot>(ImageMap->Slot);
		ImageMSlot->SetAnchors(FAnchors(0, 1, 0, 1));
		ImageMSlot->SetAlignment(FVector2D(0.0f, 1.0f));
		//ImageMSlot->SetAutoSize(true);
		ImageMSlot->SetZOrder(0);
		ImageMSlot->SetSize(MapSize);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Map is null"));

	if (LevelGameMode->PlayerIcon)
	{
		ImagePlayerIcon->SetBrushFromTexture(LevelGameMode->PlayerIcon, true);
		ImagePISlot = Cast<UCanvasPanelSlot>(ImagePlayerIcon->Slot);
		ImagePISlot->SetAnchors(FAnchors(0, 1, 0, 1));
		ImagePISlot->SetAlignment(FVector2D(0.5f, 0.5f));
		//ImagePISlot->SetAutoSize(false);
		ImagePISlot->SetZOrder(1);
		ImagePISlot->SetSize(PlayerIconSize);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("PlayerIcon is null"));

	//(0,0) is bottom left corner; (1,1) is top right corner 
	BottomAxis = CornerBottomRight->GetActorLocation() - CornerBottomLeft->GetActorLocation();
	BottomAxis -= FVector(0, 0, BottomAxis.Z);
	//UE_LOG(LogTemp, Warning, TEXT("BottomAxis = %f, %f, %f"), BottomAxis.X, BottomAxis.Y, BottomAxis.Z);

	LeftAxis = CornerTopLeft->GetActorLocation() - CornerBottomLeft->GetActorLocation();
	LeftAxis -= FVector(0, 0, LeftAxis.Z);
	//UE_LOG(LogTemp, Warning, TEXT("LeftAxis = %f, %f, %f"), LeftAxis.X, LeftAxis.Y, LeftAxis.Z);

	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();
}


void UMinimap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Do your custom tick stuff here

	FVector playerPosition = LyssaActor->GetActorLocation() - CornerBottomLeft->GetActorLocation();
	playerPosition -= FVector(0, 0, LeftAxis.Z);
	//UE_LOG(LogTemp, Log, TEXT("player position in world = %f, %f, %f"), playerPosition.X, playerPosition.Y, playerPosition.Z);

	float xPosition = FVector::DotProduct(playerPosition, BottomAxis.GetSafeNormal());
	float yPosition = FVector::DotProduct(playerPosition, LeftAxis.GetSafeNormal());
	//UE_LOG(LogTemp, Log, TEXT("player position on map world value = %f, %f"), xPosition, yPosition);
	xPosition = xPosition / BottomAxis.Size();
	yPosition = yPosition / LeftAxis.Size();
	//UE_LOG(LogTemp, Log, TEXT("player position on map = %f, %f"), xPosition, yPosition);
	xPosition *= MapSize.X;
	yPosition *= -MapSize.Y ;
	//UE_LOG(LogTemp, Log, TEXT("scaled player position on map = %f, %f"), xPosition, yPosition);

	FVector2D newPosition = FVector2D(xPosition, yPosition);
	ImagePISlot->SetPosition(newPosition);
}