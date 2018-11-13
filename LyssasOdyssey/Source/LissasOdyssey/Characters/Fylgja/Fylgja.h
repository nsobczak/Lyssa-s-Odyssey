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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FollowMousePosition();

private:
	/** Static mesh to represent the Fylgja in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fylgja", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* FylgjaMesh;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fylgja", meta = (AllowPrivateAccess = "true"))
	//	USkeletalMesh* FylgjaTop;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fylgja", meta = (AllowPrivateAccess = "true"))
	//	USkeletalMesh* FylgjaBottom;
};
