// Fill out your copyright notice in the Description page of Project Settings.

#include "Minimap.h"
#include "Minimap/MinimapCorner.h"
#include "GameModes/LevelGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Lyssa/Lyssa.h"


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
				UE_LOG(LogTemp, Error, TEXT("corner nature is wrong"));
				break;
			}
		}
		else
			UE_LOG(LogTemp, Error, TEXT("a corner is null"));
	}
}

void UMinimap::NativeConstruct()
{
	// Do some custom setup
	FindCorners();

	this->LevelGameMode = Cast<ALevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!LevelGameMode) UE_LOG(LogTemp, Error, TEXT("LevelGameMode is null"));
	this->LyssaActor = LevelGameMode->Lyssa;

	this->Map = LevelGameMode->Map;
	this->PlayerIcon = LevelGameMode->PlayerIcon;
	if (!this->Map || !this->PlayerIcon)
		UE_LOG(LogTemp, Error, TEXT("Map or PlayerIcon is null"));

	BottomAxis = (CornerBottomRight->GetActorLocation() - CornerBottomLeft->GetActorLocation()).GetSafeNormal();
	BottomAxis -= FVector(0, 0, BottomAxis.Z);
	LeftAxis = (CornerTopLeft->GetActorLocation() - CornerBottomLeft->GetActorLocation()).GetSafeNormal();
	LeftAxis -= FVector(0, 0, LeftAxis.Z);


	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();
}


void UMinimap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Do your custom tick stuff here

	//(0,0) is bottom left corner; (1,1) is top right corner 

	FVector playerPosition = CornerTopLeft->GetActorLocation() - CornerBottomLeft->GetActorLocation();
	playerPosition -= FVector(0, 0, LeftAxis.Z);

	float xPosition = FVector::DotProduct(playerPosition, BottomAxis);
	float yPosition = FVector::DotProduct(playerPosition, LeftAxis);

}