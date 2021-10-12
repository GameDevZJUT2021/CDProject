// Fill out your copyright notice in the Description page of Project Settings.

#include "CubePawn.h"
#include "Components/BoxComponent.h"

enum Actions { EForward, EBack, ELeft, ERight, ECameraTurnLeft, ECameraTurnRight };

ACubePawn::ACubePawn() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
		StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		StaticMeshComponent->SetWorldScale3D(FVector(1.0f));
	}

	StaticMeshComponent->SetupAttachment(RootComponent);

}

void ACubePawn::BeginPlay(){
	Super::BeginPlay();

}

// Called every frame
void ACubePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	if (bMoving)
	{
		// 未实现:这里需要首先检测目标点的物体,从而进行相应的处理
		// 可以利用碰撞体积来获取相应物体,或者用一张地图来存储,或是遍历所有Pawn
		FVector NewLocation = GetActorLocation() + (Direction * Speed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, LocationBeforeMove) >= 100)
		{
			bMoving = false;
			Direction = FVector(0.0f, 0.0f, 0.0f);
		}
	}

}

void ACubePawn::ControlMove(int Action, int CameraDirection) {
	bMoving = true;
	LocationBeforeMove = GetActorLocation();

	switch (Action) {
	case EForward:Direction.X = FMath::Clamp(1.0f, -1.0f, 1.0f); break;
	case EBack:Direction.X = FMath::Clamp(-1.0f, -1.0f, 1.0f); break;
	case ELeft:Direction.Y = FMath::Clamp(-1.0f, -1.0f, 1.0f); break;
	case ERight:Direction.Y = FMath::Clamp(1.0f, -1.0f, 1.0f); break;
	default:break;
	}
	// adjust move direction
	Direction = Direction.RotateAngleAxis(90.0f * CameraDirection, FVector(0.0f, 0.0f, 1.0f));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("C1: %d  Direction : %f  %f"), CameraDirection, Direction.X,Direction.Y));
}

bool ACubePawn::isMoveDone() {
	return !bMoving;
}
// some change for git test