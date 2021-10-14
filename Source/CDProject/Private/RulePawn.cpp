// Fill out your copyright notice in the Description page of Project Settings.


#include "RulePawn.h"

ARulePawn::ARulePawn() {
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

	//StaticMeshComponent->SetupAttachment(RootComponent);

}

void ARulePawn::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void ARulePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bMoving)
	{
		// 未实现:这里需要首先检测目标点的物体,从而进行相应的处理
		// 可以利用碰撞体积来获取相应物体,或者用一张地图来存储,或是遍历所有Pawn
		FVector NewLocation = GetActorLocation() + (MoveDirection * Speed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, LocationBeforeMove) >= 100)
		{
			bMoving = false;
		}
	}

}

void ARulePawn::ControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocations) {
	bMoving = true;
	LocationBeforeMove = GetActorLocation();
	MoveDirection = FVector(0.0f, 0.0f, 0.0f);

	switch (Action) {
	case EActions::Forward: MoveDirection.X = FMath::Clamp(1.0f, -1.0f, 1.0f); break;
	case EActions::Back: MoveDirection.X = FMath::Clamp(-1.0f, -1.0f, 1.0f); break;
	case EActions::Left: MoveDirection.Y = FMath::Clamp(-1.0f, -1.0f, 1.0f); break;
	case EActions::Right: MoveDirection.Y = FMath::Clamp(1.0f, -1.0f, 1.0f); break;
	default:break;
	}
	// adjust move direction to camera absolute location
	MoveDirection = MoveDirection.RotateAngleAxis(90.0f * (int)CameraAbsLocations, FVector(0.0f, 0.0f, 1.0f));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("C1: %d  Direction : %f  %f"), MoveDirection.X,MoveDirection.Y));
}

void ARulePawn::IndependentMove() {

}

bool ARulePawn::isMoveDone() {
	return !bMoving;
}