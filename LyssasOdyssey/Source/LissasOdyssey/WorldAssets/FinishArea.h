#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinishArea.generated.h"

UCLASS()
class LISSASODYSSEY_API AFinishArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinishArea(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FinishArea")
		float FARadius = 1000.0f;
	
};
