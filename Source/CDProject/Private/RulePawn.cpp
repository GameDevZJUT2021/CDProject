// Fill out your copyright notice in the Description page of Project Settings.


#include "RulePawn.h"
#include "GameInfo.h"

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

	Tag = EObjectTags::Rule;
	bWalkable = false;

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
		// δʵ��:������Ҫ���ȼ��Ŀ��������,�Ӷ�������Ӧ�Ĵ���
		// ����������ײ�������ȡ��Ӧ����,������һ�ŵ�ͼ���洢,���Ǳ�������Pawn
		FVector NewLocation = GetActorLocation() + (MoveDirection * Speed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, LocationBeforeMove) >= 99)
		{
			bMoving = false;
		}
	}

}

bool ARulePawn::BeginControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) {
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

bool ARulePawn::BeginIndependentMove() {
	return false;
}

bool ARulePawn::BeginMove(int AbsXdirection, int AbsYdirection) {
	// get GameInfo
	TActorIterator<AGameInfo> iter(GetWorld());
	checkf(iter, TEXT("There is no gameinfo"));
	AGameInfo* TempGameInfo = *iter;
	int Width = TempGameInfo->MapWidth;
	int Length = TempGameInfo->MapLength;

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
	TArray<EObjectTags> selfTags = TempGameInfo->GetObjectTags(ERuleTags::You);
	TArray<EObjectTags> pushPawnTags = TempGameInfo->GetObjectTags(ERuleTags::Push);
	this;// just for debug
	if (!DestUnitInfo.isEmpty())
	{
		for (AParentPawn* pawn : DestUnitInfo.Objects)
		{
			// rule pawn could be pushed
			if (pawn->Tag == EObjectTags::Rule)
			{
				if (!pawn->BeginMove(AbsXdirection, AbsYdirection))
					return false;
			}
			// test if the pawn at destination can walk by itself
			else if (selfTags.Find(pawn->Tag) != INDEX_NONE)
			{
				if (!pawn->BeginMove(AbsXdirection, AbsYdirection))// the pawn at destination can not walk by itself
				{
					// test if we can walk on it
					if (!pawn->bWalkable && !this->bWalkable)
						return false;
				}
			}
			// test if the pawn at destination can be pushed
			else if (pushPawnTags.Find(pawn->Tag) != INDEX_NONE)
			{
				if (!pawn->BeginMove(AbsXdirection, AbsYdirection))
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

bool ARulePawn::isMoveDone() const{
	return !bMoving;
}