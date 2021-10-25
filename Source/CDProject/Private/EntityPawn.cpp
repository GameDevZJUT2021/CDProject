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
		FVector NewLocation = GetActorLocation() + (MoveDirection * Speed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, LocationBeforeMove) >= 99) //从100改为99,修复了有时会移动99.999还继续移动,导致位置不对的错误
		{
			bMoving = false;
		}
	}
	
}

bool AEntityPawn::BeginControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) {
	FVector Direction(0.0f, 0.0f, 0.0f);

	switch (Action) {
	case EActions::Forward: Direction.X = FMath::Clamp(1.0f, -1.0f, 1.0f); break;
	case EActions::Back: Direction.X = FMath::Clamp(-1.0f, -1.0f, 1.0f); break;
	case EActions::Left: Direction.Y = FMath::Clamp(-1.0f, -1.0f, 1.0f); break;
	case EActions::Right: Direction.Y = FMath::Clamp(1.0f, -1.0f, 1.0f); break;
	default:break;
	}
	// adjust  direction to camera absolute location
	Direction = Direction.RotateAngleAxis(90.0f * static_cast<int>(CameraAbsLocation), FVector(0.0f, 0.0f, 1.0f));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Direction : %f  %f"), Direction.X, Direction.Y));

	return BeginMove(FMath::RoundToInt(Direction.X), FMath::RoundToInt(Direction.Y));
}

bool AEntityPawn::BeginIndependentMove() {
	// 已经在BeginControlledMove中设置过移动,不再移动
	if (bMoving)
		return false;

	// 遇到障碍,回头
	// 第三个参数为0,提示BeginMove调用者为IndependentMove
	if (BeginMove(FMath::RoundToInt(FaceDirection.X), FMath::RoundToInt(FaceDirection.Y), 0) == false)
	{
		FaceDirection.X = -FaceDirection.X;
		FaceDirection.Y = -FaceDirection.Y;
	}

	return true;
}

bool AEntityPawn::BeginMove(int AbsXdirection, int AbsYdirection, bool ControlledOrIndenpent) {
	// AbsXdirection and AbsYdirection are not supposed to be 0 at the same time
	if (AbsXdirection == 0 && AbsYdirection == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("something wrong!!!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, this->GetHumanReadableName());
		return true;
	}
	
	// get GameInfo
	TActorIterator<AGameInfo> iter(GetWorld());
	checkf(iter, TEXT("There is no gameinfo"));
	AGameInfo* TempGameInfo = *iter;
	int Width = TempGameInfo->MapWidth;
	int Length = TempGameInfo->MapLength;

	// 记录物体的方向
	FaceDirection.X = AbsXdirection;
	FaceDirection.Y = AbsYdirection;
	if (haveFace) // 如果是有朝向的物体则旋转
	{
		float PawnYaw = 0.0f;
		if (AbsYdirection == 0)
			PawnYaw = AbsXdirection == 1 ? 180.0f : 0.0f;
		if (AbsXdirection == 0)
			PawnYaw = AbsYdirection == 1 ? 270.0f : 90.0f;
		SetActorRotation(FRotator(0.0f, PawnYaw, 0.0f));
	}


	// calculate destination
	FVector PawnLocation = GetActorLocation();
	int x = (PawnLocation.X + (Width - 1) * 50) / 100;
	int y = (PawnLocation.Y + (Length - 1) * 50) / 100;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Location : %d  %d"), x, y));
	checkf(0 <= x && x < Width && 0 <= y && y < Length, TEXT("Location Wrong"));
	int dest_x = x + AbsXdirection;
	int dest_y = y + AbsYdirection;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dest Location : %d  %d"), dest_x, dest_y));

	// test if the destination is walkable
	// out of map test
	if (0 > dest_x || dest_x >= Width || 0 > dest_y || dest_y >= Length)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Out of Map"));
		return false;
	}

	UnitInfo DestUnitInfo = TempGameInfo->MapInfo[dest_x * Width + dest_y];
	// destination is not vacant
	
	TArray<EObjectTags> CurrentMoveTags;
	if(ControlledOrIndenpent == 1)
		CurrentMoveTags = TempGameInfo->GetObjectTags(ERuleTags::You);
	else
		CurrentMoveTags = TempGameInfo->GetObjectTags(ERuleTags::Move);

	TArray<EObjectTags> pushPawnTags = TempGameInfo->GetObjectTags(ERuleTags::Push);
	this;// just for debug
	if (!DestUnitInfo.isEmpty())
	{
		for (AParentPawn* pawn : DestUnitInfo.Objects)
		{
			// rule pawn could be pushed
			if (pawn->Tag == EObjectTags::Rule)
			{
				if (!pawn->BeginMove(AbsXdirection,AbsYdirection, ControlledOrIndenpent))
					return false;
			}
			// test if the pawn at destination can walk by itself
			else if (CurrentMoveTags.Find(pawn->Tag) != INDEX_NONE)
			{
				if (!pawn->BeginMove(AbsXdirection, AbsYdirection, ControlledOrIndenpent))// the pawn at destination can not walk by itself
				{
					// test if we can walk on it
					if (!pawn->bWalkable && !this->bWalkable)
						return false;
				}
			}
			// test if the pawn at destination can be pushed
			else if (pushPawnTags.Find(pawn->Tag) != INDEX_NONE)
			{
				if (!pawn->BeginMove(AbsXdirection, AbsYdirection, ControlledOrIndenpent))
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

	// the pawn has already begun to move
	if (bMoving)
		return true;
	// set move begin
	bMoving = true;
	LocationBeforeMove = PawnLocation;
	MoveDirection.X = AbsXdirection;
	MoveDirection.Y = AbsYdirection;
	return true;
}

bool AEntityPawn::isMoveDone() const  {
	return !bMoving;
}