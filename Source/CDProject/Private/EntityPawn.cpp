// Fill out your copyright notice in the Description page of Project Settings.

#include "EntityPawn.h"
#include "Kismet/GameplayStatics.h"
#include <AudioDeviceManager.h>
#include "MyGameInfo.h"

AEntityPawn::AEntityPawn() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = SceneComponent;

	StaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));
	if (CubeMeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
		StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		StaticMeshComponent->SetWorldScale3D(FVector(1.0f));
	}

	// Load Default Sound Effect
	static ConstructorHelpers::FObjectFinder<USoundCue> WalkSoundLoader(TEXT("/Game/Music/SoundEffect/walk_Cue"));
	if (WalkSoundLoader.Succeeded())
		WalkSound = WalkSoundLoader.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> SlideSoundLoader(TEXT("/Game/Music/SoundEffect/slide_Cue"));
	if (SlideSoundLoader.Succeeded())
		SlideSound = SlideSoundLoader.Object;
}

void AEntityPawn::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AEntityPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSliding)
	{
		FVector NewLocation = GetActorLocation() + (MoveDirection * SlideSpeed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, SlidingDestination) <= 300)
		{
			if (haveSkeletaAnimation && SlideEndAnim != nullptr)
				SkeletalMeshComponent->PlayAnimation(SlideEndAnim, false);
		}


		if (FVector::Dist(NewLocation, SlidingDestination) <= 1)
		{
			// ������Idle����,������SlideEndAnim��������
			
			SlideAudioComp->SetActive(false);
			bSliding = false;
		}
	}
	
	if (bMoving)
	{
		FVector NewLocation = GetActorLocation() + (MoveDirection * Speed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, LocationBeforeMove) >= 99) //��100��Ϊ99,�޸�����ʱ���ƶ�99.999�������ƶ�,����λ�ò��ԵĴ���
		{
			bMoving = false;

			// play idle animation if this pawn has one
			if (haveSkeletaAnimation && IdleAnim != nullptr)
				SkeletalMeshComponent->PlayAnimation(IdleAnim, true);
		}
	}

	if (bFloating)
	{
		FVector Layer2Location = GetActorLocation();
		Layer2Location.Z = Layer2Z;
		float FloatingOffset =10 * sin(FDateTime::Now().GetTimeOfDay().GetTotalMilliseconds()/500);
		SetActorLocation(Layer2Location + FVector(0, 0, FloatingOffset));
	}

	if (bFlyMoving)
	{
		FVector currLocation = GetActorLocation();
		if (bFlying)
		{
			SetActorLocation(currLocation + FVector(0, 0, FlySpeed));

			if (currLocation.Z > Layer2Z - 1)
			{
				bFlyMoving = bFlying = bFalling = false;
				bFloating = true;
			}
		}
		else if (bFalling)
		{
			SetActorLocation(currLocation - FVector(0, 0, FlySpeed));

			if (currLocation.Z < Layer1Z + 1)
				bFlyMoving = bFlying = bFalling = false;
		}
	}

	if (isSelfRotating)
	{
		FRotator currRotator =  StaticMeshComponent->GetRelativeRotation();
		currRotator.Yaw += RotateSpeed;
		StaticMeshComponent->SetRelativeRotation(currRotator);
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
	// �Ѿ���BeginControlledMove�����ù��ƶ�,�����ƶ�
	if (bMoving)
		return false;

	// �����ϰ�,��ͷ
	// ����������Ϊ0,��ʾBeginMove������ΪIndependentMove
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("something wrong at Begin Move1"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, this->GetHumanReadableName());
		return true;
	}
	
	// get GameInfo
	TActorIterator<AMyGameInfo> iter(GetWorld());
	checkf(iter, TEXT("There is no gameinfo"));
	AMyGameInfo* CurrGameInfo = *iter;
	int Width = CurrGameInfo->MapWidth;
	int Length = CurrGameInfo->MapLength;

	// ��¼����ķ���
	FaceDirection.X = AbsXdirection;
	FaceDirection.Y = AbsYdirection;
	if (haveFace) // ������г������������ת��Ŀ�귽��
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

	FUnitInfo DestUnitInfo = CurrGameInfo->MapInfo[dest_x * Width + dest_y];

	// �ܹ�slide����������һ�񻬶��Ŀռ�
	if (CurrGameInfo->ActiveRules.FindPair(ERuleTags::Slide, static_cast<ERuleTags>(this->Tag))
		&& DestUnitInfo.isEmpty())
	{
		int SlideStack = 0;
		while (0 <= dest_x && dest_x < Width && 0 <= dest_y && dest_y < Length)
		{
			DestUnitInfo = CurrGameInfo->MapInfo[dest_x * Width + dest_y];

			bool shouldStop = false;
			if (!DestUnitInfo.isEmpty())
			{
				for (auto& pawn : DestUnitInfo.Objects)
				{
					if (pawn->Tag == EObjectTags::Rule)
					{
						shouldStop = true;
						break;
					}
					else if (pawn->bWalkable == false)
					{
						if (CurrGameInfo->ActiveRules.FindPair(ERuleTags::Slide, static_cast<ERuleTags>(pawn->Tag)))//��һ���Pawnһ�𻬶�
							SlideStack++;
						else
							shouldStop = true;
					}
				}
			}

			if(shouldStop)
				break;

			dest_x += AbsXdirection;
			dest_y += AbsYdirection;
		}

		dest_x -= SlideStack * AbsXdirection;
		dest_y -= SlideStack * AbsYdirection;


		SlidingDestination.X = 100 * (dest_x - AbsXdirection) - (Width - 1) * 50;
		SlidingDestination.Y = 100 * (dest_y - AbsYdirection) - (Length -1) * 50;
		SlidingDestination.Z = GetActorLocation().Z;

		bSliding = true;
		MoveDirection.X = AbsXdirection;
		MoveDirection.Y = AbsYdirection;

		SkeletalMeshComponent->PlayAnimation(SlideBeginAnim, false);

		// play slide music effect
		if (SlideSound)// keep reference of that sound cue in order to stop it
			SlideAudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), SlideSound);

		return true;
	}
	// ����slide�ͽ�����ͨ���ж�
	else
	{
		// destination is not vacant
		TArray<EObjectTags> CurrentMoveTags;
		if (ControlledOrIndenpent == 1)
			CurrentMoveTags = CurrGameInfo->GetObjectTags(ERuleTags::You);
		else
			CurrentMoveTags = CurrGameInfo->GetObjectTags(ERuleTags::Move);

		TArray<EObjectTags> pushPawnTags = CurrGameInfo->GetObjectTags(ERuleTags::Push);

		if (!DestUnitInfo.isEmpty())
		{
			for (AParentPawn* pPawn : DestUnitInfo.Objects)
			{
				if (pPawn->onLayer != onLayer)
					continue;

				// rule pawn could be pushed
				if (pPawn->Tag == EObjectTags::Rule)
				{
					if (!pPawn->BeginMove(AbsXdirection, AbsYdirection, ControlledOrIndenpent))
						return false;
				}
				// test if the pawn at destination can walk by itself
				else if (CurrentMoveTags.Find(pPawn->Tag) != INDEX_NONE)
				{
					if (!pPawn->BeginMove(AbsXdirection, AbsYdirection, ControlledOrIndenpent))// the pawn at destination can not walk by itself
					{
						// test if we can walk on it
						if (!pPawn->bWalkable && !this->bWalkable)
							return false;
					}
				}
				// test if the pawn at destination can be pushed
				else if (pushPawnTags.Find(pPawn->Tag) != INDEX_NONE)
				{
					if (!pPawn->BeginMove(AbsXdirection, AbsYdirection, ControlledOrIndenpent))
						return false;
				}
				// destination has a pawn that we can not walk on
				else if (this->Tag != pPawn->Tag)
				{
					if (!pPawn->bWalkable && !this->bWalkable)
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

		// play animation if this pawn has one
		if (haveSkeletaAnimation)
		{
			static bool walk_left_foot = 1;
			if (walk_left_foot)
			{
				SkeletalMeshComponent->PlayAnimation(WalkAnim1, false);
				walk_left_foot = 0;
			}
			else
			{
				SkeletalMeshComponent->PlayAnimation(WalkAnim2, false);
				walk_left_foot = 1;
			}

		}

		// play walk music effect
		if(WalkSound)
			UGameplayStatics::PlaySound2D(GetWorld(), WalkSound);

		return true;
	}
}

bool AEntityPawn::isMoveDone() const  {
	return !bMoving && !bFlyMoving && !bSliding;
}

void AEntityPawn::BeginFlyOrFall() {
	bFlyMoving = true;
	// Obtain GameInfo
	TActorIterator<AMyGameInfo> iterGameInfo(GetWorld());
	checkf(iterGameInfo, TEXT("There is no GameInfo"));

	if (GetActorLocation().Z < Layer1Z + 1)//���ڵ�һ��,��ʼ����
	{
		bFlying = true;
		onLayer = 2;
	}
	else if ( ! iterGameInfo->OnLayer2Land(this))//���ڵڶ���,��������ڵڶ����½����,������½�
	{
		FallingDown();
	}
}

void AEntityPawn::FallingDown() {
	//��ʼ�½�,�½�ǰ�Ƚ��߶ȵ�����layer2Z���������Ϊ��ɫƯ�����������
	bFlyMoving = true;
	FVector currLocation = GetActorLocation();
	currLocation.Z = Layer2Z;
	SetActorLocation(currLocation);
	bFalling = true;
	bFloating = false;
	onLayer = 1;
}
