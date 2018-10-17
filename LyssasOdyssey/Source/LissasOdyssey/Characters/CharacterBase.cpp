#include "CharacterBase.h"


// Sets default values
ACharacterBase::ACharacterBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	Life = MaxLife;
}

float ACharacterBase::GetCurrentLife()
{
	return Life;
}

void ACharacterBase::UpdateLife(float lifeChange)
{
	Life += lifeChange;
	if (Life > MaxLife)
	{
		Life = MaxLife;
	}
}

//________________________________________________________________________

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//// Called to bind functionality to input
//void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

