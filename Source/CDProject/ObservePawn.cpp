// Fill out your copyright notice in the Description page of Project Settings.


#include "ObservePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "CommonEnum.h"
#include "Camera/CameraComponent.h"

// Sets default values
AObservePawn::AObservePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	// A camera componont is enough
	if (!RootComponent)
		RootComponent = TopDownCameraComponent;
}

// Called when the game starts or when spawned
void AObservePawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AObservePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotating)
	{
		static int RotatedAngle = 0;

		// Rotate finish
		if (RotatedAngle >= 90)
		{
			RotatedAngle = 0;

			// Round to prevent the  accumulation of float error
			FVector Radius = GetActorLocation();
			SetActorLocation(FVector(round(Radius.X),round(Radius.Y),Radius.Z));

			bRotating = false;
			return;
		}

		// Change location
		FVector Radius = GetActorLocation();
		FVector RotateValue = Radius.RotateAngleAxis(RotateDirection * RotateSpeed, RotateAxis);
		SetActorLocation(RotateValue);

		// Change Rotation, make the camera looking at the center
		FRotator Rotator = GetActorRotation();
		Rotator.Yaw += RotateDirection * RotateSpeed;
		SetActorRotation(Rotator);

		RotatedAngle += RotateSpeed;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT(" %d "),RotatedAngle));

		if (RotateSpeed > 3)// Rotate smoothly rather than linearly
			RotateSpeed = RotateSpeed - 1;
	}
}

// Called to bind functionality to input
void AObservePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AObservePawn::BeginRotate(EActions Action) {
	bRotating = true;
	RotateSpeed = 8.0f;
	Action == EActions::CameraTurnLeft ? RotateDirection = 1 : RotateDirection = -1;
}

bool AObservePawn::isRotateDone() {
	return !bRotating;
}

void AObservePawn::Activate() {
	bActivate = 1;
}