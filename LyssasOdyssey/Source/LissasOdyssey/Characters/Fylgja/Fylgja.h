#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h" 
#include "Utils/Structures/StructStickInputs.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fylgja")
		bool IsVisible = true;

	UFUNCTION(BlueprintCallable, Category = "Fylgja")
		void ShowFylfja();
	UFUNCTION(BlueprintCallable, Category = "Fylgja")
		void HideFylfja();

	UFUNCTION(BlueprintCallable, Category = "Fylgja")
		void UpdateFRotation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fylgja")

		float fLeftKeyValue = 0, fTopKeyValue = 0, fRightKeyValue = 0, fDownKeyValue = 0;
	float fRotationAngle = 0;

protected:
	UFUNCTION()
		void FollowMousePosition();

private:
	/** Static mesh to represent the Fylgja in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fylgja", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* FylgjaMesh;

	UPROPERTY(VisibleAnywhere, Category = "Fylgja")
		TArray<FStructStickInputs> FRotationValues;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fylgja", meta = (AllowPrivateAccess = "true"))
	//	USkeletalMesh* FylgjaTop;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fylgja", meta = (AllowPrivateAccess = "true"))
	//	USkeletalMesh* FylgjaBottom;

};
