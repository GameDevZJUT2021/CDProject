// Copyright Epic Games, Inc. All Rights Reserved.

#include "CDProjectPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "CDProjectCharacter.h"
#include "Engine/World.h"

enum Actions { EForward, EBack, ELeft, ERight, ECameraTurnLeft, ECameraTurnRight };

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

		// Spawn a test Pawn
		Location = FVector(0.0f, 0.0f, 50.0f);
		Rotation = FRotator(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = 1;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		TestPawn = World->SpawnActor<ACubePawn>(Location, Rotation, SpawnParams);
	}

	// get GameInfo
	for (TObjectIterator<AGameInfo> Itr; Itr; ++Itr)
	{
		MyGameInfo = *Itr;
	}
}

void ACDProjectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	this->SetViewTarget(MyObservePawn);

	static int Action;
	if (!bOperatingAction)
	{
		if (ActionQueue.Dequeue(Action))
		{
			bOperatingAction = true;
			switch (Action)
			{
			case EForward:
			case EBack:
			case ELeft:
			case ERight:
				ProcessMoveAction(Action);
				TestPawn->ControlMove(Action, CameraDirection);
				break;
			case ECameraTurnLeft:
			case ECameraTurnRight:
				MyObservePawn->BeginRotate(Action);
				break;
			default:break;
			}
			//{
				/*
				for (TObjectIterator<ACubePawn> Itr; Itr; ++Itr)
				{
					ACubePawn* curr = *Itr;
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, curr->GetHumanReadableName());
				}
				*/
				/*
				��ʵ��

				���ݹ�����ȡ��ǰӦ�ò�����Pawn��������
				TArray<Pawn*> ControlPawn = GetControlPawn();
				possess���ж�Ӧ��Pawn,�����ƶ�����,ÿ��Pawn���Լ����ƶ������ڴ�����ײ�Ȳ���
				for(every Pawn in ControlPawn)
				{
					Possess(Pawn)// ������ҪPossess,����ֱ�ӽ��к������ôӶ����в���
					Pawn->ControlMove(Action);// ����ĸı���������
				}
				Possess(ObserverPawn)

				���ݹ�����ȡ��ǰӦ��Move��Pawn��������
				TArray<Pawn*> MovePawn = GetMovePawn();
				for(every Pawn in MovePawn)
				{
					Possess(Pawn)
					Pawn->SelfMove();// ����ĸı�������
				}
				Possess(ObserverPawn)
				*/
				
				// just for test
				
			//}
		}
		else // There is no Action input
			return;
	}
	else
	{
		switch (Action)
		{
		case EForward:
		case EBack:
		case ELeft:
		case ERight:
			if (ProcessMoveActionDone())
			{

			}
			if (TestPawn->isMoveDone())
			{
				bOperatingAction = false;
				// δʵ��:һ�β���������,���¹����.
			}
			break;
		case ECameraTurnLeft:
		case ECameraTurnRight:
			if (MyObservePawn->isRotateDone())
			{
				bOperatingAction = false;
				Action == ECameraTurnLeft ? CameraDirection++ : CameraDirection--;
				//UpdateRule();
			}
			break;
		default:break;
		}
	}

}

void ACDProjectPlayerController::ProcessMoveAction(int Action) {
	//TArray selfsTag= GetSelf
	//for every selfTag
	//    find pawn-ptr
	//for every pawn-ptr
	//    pawn-ptr->move()
	
}

bool ACDProjectPlayerController::ProcessMoveActionDone() {
	return true;
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
	ActionQueue.Enqueue(EForward);
}

void ACDProjectPlayerController::OnMoveBack() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Move Back")));
	ActionQueue.Enqueue(EBack);
}

void ACDProjectPlayerController::OnMoveLeft() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Move Left")));
	ActionQueue.Enqueue(ELeft);
}

void ACDProjectPlayerController::OnMoveRight() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Move Right")));
	ActionQueue.Enqueue(ERight);
}

void ACDProjectPlayerController::OnCameraTurnLeft() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Turn Left")));
	ActionQueue.Enqueue(ECameraTurnLeft);
}

void ACDProjectPlayerController::OnCameraTurnRight() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Turn Right")));
	ActionQueue.Enqueue(ECameraTurnRight);
}
