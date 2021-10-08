// Fill out your copyright notice in the Description page of Project Settings.


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


void AParentPawn::ControlMove(int Action,int CameraDirection) {

}

void AParentPawn::SelfMove() {

}

bool AParentPawn::isMoveDone() {
	return true;
}
