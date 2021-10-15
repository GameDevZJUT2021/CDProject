// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityPawn.h"
#include "GameInfo.h"

AEntityPawn::AEntityPawn() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));
	if (CubeMeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
		StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		StaticMeshComponent->SetWorldScale3D(FVector(1.0f));
	}

}

void AEntityPawn::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AEntityPawn::Tick(float DeltaTime)
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

bool AEntityPawn::ControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) {
	// get GameInfo
	TActorIterator<AGameInfo> iter(GetWorld());
	checkf(iter, TEXT("There is no gameinfo"));
	AGameInfo* TempGameInfo = *iter;
	int Width = TempGameInfo->MapWidth;
	int Length = TempGameInfo->MapLength;

	FVector PawnLocation = GetActorLocation();
	int x = (PawnLocation.X + (Width-1)*50)/100;
	int y = (PawnLocation.Y + (Length-1)*50)/100;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Location : %d  %d"), x, y));
	checkf(0 <= x && x <Width && 0 <= y && y < Length, TEXT("Location Wrong"));

	MoveDirection = FVector(0.0f, 0.0f, 0.0f);

	switch (Action) {
	case EActions::Forward: MoveDirection.X = FMath::Clamp(1.0f, -1.0f, 1.0f); break;
	case EActions::Back: MoveDirection.X = FMath::Clamp(-1.0f, -1.0f, 1.0f); break;
	case EActions::Left: MoveDirection.Y = FMath::Clamp(-1.0f, -1.0f, 1.0f); break;
	case EActions::Right: MoveDirection.Y = FMath::Clamp(1.0f, -1.0f, 1.0f); break;
	default:break;
	}
	// adjust move direction to camera absolute location
	MoveDirection = MoveDirection.RotateAngleAxis(90.0f * (int)CameraAbsLocation, FVector(0.0f, 0.0f, 1.0f));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Direction : %f  %f"), MoveDirection.X,MoveDirection.Y));

	// test if the destination is walkable
	int dest_x = x + MoveDirection.X;
	int dest_y = y + MoveDirection.Y;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dest Location : %d  %d"), dest_x, dest_y));

	// out of map test
	if (0 > dest_x || dest_x >= Width || 0 > dest_y || dest_y >= Length)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Out of Map"));
		return false;
	}

	UnitInfo DestUnitInfo = TempGameInfo->MapInfo[dest_x * Width + dest_y];
	// destination is not vacant
	if (!DestUnitInfo.isEmpty())
	{
		// 没有考虑push性质
		for (AParentPawn* pawn : DestUnitInfo.Objects)
		{
			// rule pawn could be pushed
			if (pawn->Tag == EObjectTags::Rule)
			{
				if (!pawn->ControlledMove(Action, CameraAbsLocation))
					return false;
			}


			// destination has a pawn that we can not walk on
			else if (this->Tag != pawn->Tag)
			{
				if (!pawn->bWalkable && !this->bWalkable)
					return false;
			}

		}
	}

	bMoving = true;
	LocationBeforeMove = PawnLocation;
	return true;
}

void AEntityPawn::IndependentMove() {

}

bool AEntityPawn::isMoveDone() {
	return !bMoving;
}