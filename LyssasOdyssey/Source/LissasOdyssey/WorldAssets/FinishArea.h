#pragma once

#include "CoreMinimal.h"
#include "Utils/ActorWithMeshBase.h"
#include "FinishArea.generated.h"

UCLASS()
class LISSASODYSSEY_API AFinishArea : public AActorWithMeshBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinishArea(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsLevelFinished;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
