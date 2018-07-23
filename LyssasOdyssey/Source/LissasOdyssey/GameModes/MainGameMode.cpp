#include "MainGameMode.h"


AMainGameMode::AMainGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ALyssa::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("/Game/Characters/Lyssa/BPLyssa"));
	if (PlayerPawnObject.Succeeded())
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}

	//HUDClass = AFPSHUD::StaticClass();
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bAutoManageActiveCameraTarget = false;

	//label
	TArray<FString> tmpMainSettingsLabel;
	tmpMainSettingsLabel.Append(MainSettingsLabel, ARRAY_COUNT(MainSettingsLabel));
	for (size_t i = 0; i < tmpMainSettingsLabel.Num(); i++)
	{
		TAMainSettingsLabel.Add(FText::FromString(tmpMainSettingsLabel[i]));
	}

	TArray<FString> tmpFPSSettingsLabel;
	tmpFPSSettingsLabel.Append(FPSSettingsLabel, ARRAY_COUNT(FPSSettingsLabel));
	for (size_t i = 0; i < tmpFPSSettingsLabel.Num(); i++)
	{
		TAFPSSettingsLabel.Add(FText::FromString(tmpFPSSettingsLabel[i]));
	}

	TArray<FString> tmpResolutionLabel;
	tmpResolutionLabel.Append(ResolutionLabel, ARRAY_COUNT(ResolutionLabel));
	for (size_t i = 0; i < tmpResolutionLabel.Num(); i++)
	{
		TAResolutionLabel.Add(FText::FromString(tmpResolutionLabel[i]));
	}

	//command
	TAGraphicalCommands.Append(GraphicalCommands, ARRAY_COUNT(GraphicalCommands));
	TAPPCommands.Append(GraphicalCommands, ARRAY_COUNT(PPCommands));
	TAAACommands.Append(GraphicalCommands, ARRAY_COUNT(AACommands));
	TAShadowCommands.Append(GraphicalCommands, ARRAY_COUNT(ShadowCommands));
	TAFPSCommands.Append(GraphicalCommands, ARRAY_COUNT(FPSCommands));
	TAResCommands.Append(ResCommands, ARRAY_COUNT(ResCommands));

	//widget
	//ChangeMenuWidget(StartingWidgetClass);


	//TMP
	FString Final0 = "r.ScreenPercentage 50";
	GetWorld()->Exec(GetWorld(), *Final0);
	FString Final1 = "sg.PostProcessQuality 1";
	GetWorld()->Exec(GetWorld(), *Final1);
	FString Final2 = "r.PostProcessAAQuality 1";
	GetWorld()->Exec(GetWorld(), *Final2);
	FString Final3 = "sg.ShadowQuality 1";
	GetWorld()->Exec(GetWorld(), *Final3);
	FString Final4 = "t.MaxFPS 30";
	GetWorld()->Exec(GetWorld(), *Final4);
}


//void AMainGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
//{
//	if (CurrentWidget != nullptr)
//	{
//		//UE_LOG(LogTemp, Warning, TEXT("current widget not null"));
//
//		CurrentWidget->RemoveFromViewport();
//		CurrentWidget = nullptr;
//	}
//
//	if (NewWidgetClass != nullptr)
//	{
//		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
//		if (CurrentWidget != nullptr)
//		{
//			CurrentWidget->AddToViewport();
//		}
//	}
//}

void AMainGameMode::ChangeGraphicSetting(GraphicLabel graphicLabel, bool increase) {
	TArray <FString> commandList;
	int32 commandIndex = 0;

	//index
	switch (graphicLabel)
	{
	case Graphical:
		increase ? ++GraphicalIndex : --GraphicalIndex;
		commandList = TAGraphicalCommands;
		GraphicalIndex = (GraphicalIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(GraphicalIndex < 0) ? 0 : GraphicalIndex;
		commandIndex = GraphicalIndex;
		break;

	case PP:
		increase ? ++PPIndex : --PPIndex;
		commandList = TAPPCommands;
		PPIndex = (PPIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(PPIndex < 0) ? 0 : PPIndex;
		commandIndex = PPIndex;
		break;

	case AA:
		increase ? ++AAIndex : --AAIndex;
		commandList = TAAACommands;
		AAIndex = (AAIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(AAIndex < 0) ? 0 : AAIndex;
		commandIndex = AAIndex;
		break;

	case Shadow:
		increase ? ++ShadowIndex : --ShadowIndex;
		commandList = TAShadowCommands;
		ShadowIndex = (ShadowIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(ShadowIndex < 0) ? 0 : ShadowIndex;
		commandIndex = ShadowIndex;
		break;

	case FPS:
		increase ? ++FPSIndex : --FPSIndex;
		commandList = TAFPSCommands;
		FPSIndex = (FPSIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(FPSIndex < 0) ? 0 : FPSIndex;
		commandIndex = FPSIndex;
		break;

	case Res:
		increase ? ++ResIndex : --ResIndex;
		commandList = TAResCommands;
		ResIndex = (ResIndex > commandList.Num() - 1) ? commandList.Num() - 1 :
			(ResIndex < 0) ? 0 : ResIndex;
		commandIndex = ResIndex;
		break;

	default:
		break;
	}

	//command
	if (commandIndex >= 0 && commandIndex < commandList.Num()) //check for safety
		GetWorld()->Exec(GetWorld(), *(commandList[commandIndex]));
}


