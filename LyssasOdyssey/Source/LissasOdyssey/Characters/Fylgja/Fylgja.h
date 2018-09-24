#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h" 
#include "Fylgja.generated.h"

UCLASS()
class LISSASODYSSEY_API AFylgja : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFylgja(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FollowMousePosition();
	
	
};
