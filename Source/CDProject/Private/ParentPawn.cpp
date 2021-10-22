#include "ParentPawn.h"

// Sets default values
AParentPawn::AParentPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParentPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParentPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AParentPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


bool AParentPawn::BeginControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) {
	return false;
}

bool AParentPawn::BeginIndependentMove() {
	return false;
}

bool AParentPawn::isMoveDone() const {
	return true;
}

bool AParentPawn::BeginMove(int AbsXdirection, int AbsYdirection){
	return false;
}
