// Copyright Epic Games, Inc. All Rights Reserved.

#include "CDProjectPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "CDProjectCharacter.h"
#include "EntityPawn.h"
#include "EngineUtils.h"
#include "Engine/World.h"

ACDProjectPlayerController::ACDProjectPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

}

void ACDProjectPlayerController::BeginPlay() {
	UWorld* World = GetWorld();
	if (World)
	{
		// Spawn a ObserveCamera
		FVector Location = FVector(-1800.0f, 0.0f, 1500.0f);
		FRotator Rotation = FRotator(-50.0f, 0.0f, 0.0f);
		MyObservePawn = World->SpawnActor<AObservePawn>(Location, Rotation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = 1;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// Spawn a test Walkable Entity Pawn Water
		Location = FVector(200.0f, 0.0f, 55.0f);// a little bit higher than 50, avoid collision with ground
		Rotation = FRotator(0.0f, 0.0f, 0.0f);
		AEntityPawn* WaterEntityPawn = World->SpawnActor<AEntityPawn>(Location, Rotation, SpawnParams);
		WaterEntityPawn->Tag = EObjectTags::Water;
		WaterEntityPawn->bWalkable = true;

		// Spawn a test UnWalkable Entity Pawn Tree
		Location = FVector(0.0f, 0.0f, 55.0f);
		Rotation = FRotator(0.0f, 0.0f, 0.0f);
		AEntityPawn* TreeEntityPawn = World->SpawnActor<AEntityPawn>(Location, Rotation, SpawnParams);
		TreeEntityPawn->Tag = EObjectTags::Tree;
		TreeEntityPawn->bWalkable = false;

		// Spawn a test Walkable Entity Pawn Baba
		Location = FVector(200.0f, 200.0f, 55.0f);
		Rotation = FRotator(0.0f, 0.0f, 0.0f);
		AEntityPawn* BabaEntityPawn = World->SpawnActor<AEntityPawn>(Location, Rotation, SpawnParams);
		BabaEntityPawn->Tag = EObjectTags::Baba;
		BabaEntityPawn->bWalkable = false;

		// Spawn GameInfo
		MyGameInfo = World->SpawnActor<AGameInfo>();
		MyGameInfo->Init(25, 25);
	}
}

void ACDProjectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	this->SetViewTarget(MyObservePawn);

	static EActions Action;
	if (!bOperatingAction)
	{
		if (ActionQueue.Dequeue(Action))
		{
			bOperatingAction = true;
			switch (Action)
			{
			
			case EActions::Forward:
			case EActions::Back:
			case EActions::Left:
			case EActions::Right:
				ProcessMoveAction(Action);
				break;

			case EActions::CameraTurnLeft:
			case EActions::CameraTurnRight:
				MyObservePawn->BeginRotate(Action);
				break;

			default:break;
			}
		}
		else // There is no Action input
			return;
	}
	else
	{
		switch (Action)
		{

		case EActions::Forward:
		case EActions::Back:
		case EActions::Left:
		case EActions::Right:
			if (ProcessMoveActionDone())
			{
				bOperatingAction = false;
				//MyGameInfo->UpdateRule();
			}
			break;

		case EActions::CameraTurnLeft:
		case EActions::CameraTurnRight:
			if (MyObservePawn->isRotateDone())
			{
				UpdateCameraAbsLocation(Action);
				bOperatingAction = false;
				//UpdateRule();
			}
			break;

		default:break;
		}
	}

}


void ACDProjectPlayerController::ProcessMoveAction(EActions Action) {
	checkf(MyGameInfo, TEXT("we are not holding GameInfo"));

	int i = 1;
	for (TActorIterator<AEntityPawn> iter(GetWorld()); iter; ++iter)
	{
		AEntityPawn* EntityPawn = *iter;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("pawn count %d"), i++));
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, EntityPawn->GetHumanReadableName());
		EntityPawn->ControlledMove(Action, CameraAbsLocation);
	}
	//TArray<AParentPawn*> SelfPawns = MyGameInfo->GetSelfPawns();
	//for (AParentPawn* SelfPawn : SelfPawns)
	//	SelfPawn->ControlledMove(Action,CameraAbsLocation);

	//TArray<AParentPawn*> SelfPawns = MyGameInfo->GetMovePawns();
	//for (AParentPawn* SelfPawn : SelfPawns)
	//	SelfPawn->IndependentMove(Action);
}


bool ACDProjectPlayerController::ProcessMoveActionDone() {
	for (TActorIterator<AEntityPawn> iter(GetWorld()); iter; ++iter)
	{
		AEntityPawn* EntityPawn = *iter;
		if (!EntityPawn->isMoveDone()) return false;
	}
	return true;
}


void ACDProjectPlayerController::UpdateCameraAbsLocation(EActions Action) {
	int location = (int)CameraAbsLocation;
	location += Action == EActions::CameraTurnLeft ? 1 : -1;
	location = location < 0 ? 3 : location % 4;
	CameraAbsLocation = (ECameraAbsLocations)location;
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("CameAbsLocation: %d"),(int)CameraAbsLocation));
}


// Input
void ACDProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("MoveForward", IE_Pressed, this, &ACDProjectPlayerController::OnMoveForward);
	InputComponent->BindAction("MoveBack", IE_Pressed, this, &ACDProjectPlayerController::OnMoveBack);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ACDProjectPlayerController::OnMoveLeft);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &ACDProjectPlayerController::OnMoveRight);

	InputComponent->BindAction("CameraTurnLeft", IE_Pressed, this, &ACDProjectPlayerController::OnCameraTurnLeft);
	InputComponent->BindAction("CameraTurnRight", IE_Pressed, this, &ACDProjectPlayerController::OnCameraTurnRight);
}


void ACDProjectPlayerController::OnMoveForward() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Move Forward")));
	ActionQueue.Enqueue(EActions::Forward);
}

void ACDProjectPlayerController::OnMoveBack() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Move Back")));
	ActionQueue.Enqueue(EActions::Back);
}

void ACDProjectPlayerController::OnMoveLeft() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Move Left")));
	ActionQueue.Enqueue(EActions::Left);
}

void ACDProjectPlayerController::OnMoveRight() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Move Right")));
	ActionQueue.Enqueue(EActions::Right);
}

void ACDProjectPlayerController::OnCameraTurnLeft() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Turn Left")));
	ActionQueue.Enqueue(EActions::CameraTurnLeft);
}

void ACDProjectPlayerController::OnCameraTurnRight() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Turn Right")));
	ActionQueue.Enqueue(EActions::CameraTurnRight);
}
