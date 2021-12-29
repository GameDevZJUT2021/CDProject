// Copyright Epic Games, Inc. All Rights Reserved.

#include "CDProjectPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "EntityPawn.h"
#include "RulePawn.h"
#include "EngineUtils.h"
#include "Misc/OutputDeviceNull.h"
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
		// Obtain a ObserveCamera
		TActorIterator<AObservePawn> iterObservePawn(GetWorld());
		checkf(iterObservePawn, TEXT("There is no ObserverPawn"));
		MyObservePawn = *iterObservePawn;

		// Obtain GameInfo
		TActorIterator<AMyGameInfo> iterGameInfo(GetWorld());
		checkf(iterGameInfo, TEXT("There is no GameInfo"));
		MyGameInfo = *iterGameInfo;
		MyGameInfo->Init();
	}
}

void ACDProjectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	if(MyObservePawn->bActivate)
		this->SetViewTarget(MyObservePawn);

	static EActions Action;
	if (!bOperatingAction)
	{
		if (!MyObservePawn->bActivate)
		{
			// 当前为相机激活后处理之前的动作
			// ActionQueue.Dequeue(Action); //取消注释,则相机激活后丢弃之前的动作
			return;
		}
		if (ActionQueue.Dequeue(Action))
		{
			bOperatingAction = true;
			switch (Action)
			{
			
			case EActions::Forward:
			case EActions::Back:
			case EActions::Left:
			case EActions::Right:
			case EActions::Wait:
			case EActions::FlyOrFall:
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
		case EActions::Wait:
		case EActions::FlyOrFall:
			if (ProcessMoveActionDone())
			{
				bOperatingAction = false;
				bWaiting = false;
				MyGameInfo->UpdateMapInfo();
				MyGameInfo->UpdateRule(CameraAbsLocation);
				if (MyGameInfo->WinJudge())
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString(TEXT("You Win")));
					FOutputDeviceNull OutputDeviceNull;
					CallFunctionByNameWithArguments(TEXT("Function_UMG_CreateNewWindow_Win"), OutputDeviceNull, nullptr, true);
				}
				if (MyGameInfo->DefeatJudge())
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("You Lose")));
					FOutputDeviceNull OutputDeviceNull;
					CallFunctionByNameWithArguments(TEXT("Function_UMG_CreateNewWindow_Lose"), OutputDeviceNull, nullptr, true);
				}
			}
			break;

		case EActions::CameraTurnLeft:
		case EActions::CameraTurnRight:
			if (MyObservePawn->isRotateDone())
			{
				UpdateCameraAbsLocation(Action);
				bOperatingAction = false;
				MyGameInfo->UpdateRule(CameraAbsLocation);
				if (MyGameInfo->WinJudge())
				{
					// 弹出胜利窗口
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString(TEXT("You Win")));

				}
				if (MyGameInfo->DefeatJudge())
				{
					// 弹出失败窗口
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("You Lose")));

				}
			}
			break;

		default:break;
		}
	}

}


void ACDProjectPlayerController::ProcessMoveAction(EActions Action) {
	checkf(MyGameInfo, TEXT("we are not holding GameInfo"));

	TArray<AEntityPawn*> pYouPawns = MyGameInfo->GetSelfPawns();
	TArray<AEntityPawn*> pMovePawns = MyGameInfo->GetMovePawns();

	if (Action == EActions::FlyOrFall)
	{
		TArray<EObjectTags> flyObjectTags = MyGameInfo->GetObjectTags(ERuleTags::Fly);
		for (AEntityPawn* pYouPawn : pYouPawns)
		{
			if (flyObjectTags.Find(pYouPawn->Tag) != INDEX_NONE)// 具有fly的属性
				pYouPawn->BeginFlyOrFall();
		}
		return;
	}

	if (Action == EActions::Wait)
		bWaiting = true;

	// 失去操控
	if (pYouPawns.Num() == 0  && pMovePawns.Num() == 0)
	{
		// 弹出失败窗口
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("You Lost Control")));
		FOutputDeviceNull OutputDeviceNull;
		CallFunctionByNameWithArguments(TEXT("Function_UMG_CreateNewWindow_Lose"), OutputDeviceNull, nullptr, true);

	}

	if(!bWaiting)
		for(AEntityPawn*  pYouPawn : pYouPawns)
			pYouPawn->BeginControlledMove(Action, CameraAbsLocation);
	
	for (AEntityPawn* pMovePawn : pMovePawns)
		pMovePawn->BeginIndependentMove();

}


bool ACDProjectPlayerController::ProcessMoveActionDone() {
	for (TActorIterator<AEntityPawn> iter(GetWorld()); iter; ++iter)
	{
		AEntityPawn* EntityPawn = *iter;
		if (!EntityPawn->isMoveDone()) return false;
	}

	// 此时发出的所有动作已经完成,但是还要检测是否有物体会掉落
	// 再次检测时不会再执行到这里,因为isMoveDone也包含了Falling的动作
	bool retVal = true;
	TArray<EObjectTags> flyObjectTags = MyGameInfo->GetObjectTags(ERuleTags::Fly);
	for (TActorIterator<AParentPawn> iter(GetWorld()); iter; ++iter)
	{
		AParentPawn* pawn = *iter;
		if (!MyGameInfo->OnLayer2Land(pawn) && pawn->GetActorLocation().Z + 1 > Layer2Z)
		{
			if (pawn->Tag == EObjectTags::Rule || flyObjectTags.Find(pawn->Tag) == INDEX_NONE)//这里要对所有要下落的物体发出下落的信号
			{
				pawn->FallingDown();
				retVal = false;
			}
		}
	}
	return retVal;
}


void ACDProjectPlayerController::UpdateCameraAbsLocation(EActions Action) {
	int location = static_cast<int>(CameraAbsLocation);
	location += Action == EActions::CameraTurnLeft ? 1 : -1;
	location = location < 0 ? 3 : location % 4;
	CameraAbsLocation = static_cast<ECameraAbsLocations>(location);
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

	InputComponent->BindAction("Wait", IE_Pressed, this, &ACDProjectPlayerController::OnWait);

	InputComponent->BindAction("Fly", IE_Pressed, this, &ACDProjectPlayerController::OnFly);

	InputComponent->BindAction("CameraTurnLeft", IE_Pressed, this, &ACDProjectPlayerController::OnCameraTurnLeft);
	InputComponent->BindAction("CameraTurnRight", IE_Pressed, this, &ACDProjectPlayerController::OnCameraTurnRight);

	//InputComponent->RemoveActionBinding("MoveRight", IE_Pressed);
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

void ACDProjectPlayerController::OnWait() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Wait")));
	bWaiting = true;
	ActionQueue.Enqueue(EActions::Wait);
}

void ACDProjectPlayerController::OnFly() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Fly")));
	bWaiting = true;
	ActionQueue.Enqueue(EActions::FlyOrFall);
}

void ACDProjectPlayerController::OnCameraTurnLeft() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Turn Left")));
	ActionQueue.Enqueue(EActions::CameraTurnLeft);
}

void ACDProjectPlayerController::OnCameraTurnRight() {
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Turn Right")));
	ActionQueue.Enqueue(EActions::CameraTurnRight);
}
