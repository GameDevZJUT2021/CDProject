// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ObservePawn.h"
#include "CubePawn.h"
#include "GameInfo.h"
#include "CDProjectPlayerController.generated.h"

UCLASS()
class ACDProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACDProjectPlayerController();

protected:
	TQueue<int> ActionQueue;
	bool bOperatingAction = 0;

	int CameraDirection = 0;

	AObservePawn* MyObservePawn;

	ACubePawn* TestPawn;

	AGameInfo* MyGameInfo;

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void ProcessMoveAction(int Action);
	bool ProcessMoveActionDone();

	void OnMoveLeft();
	void OnMoveRight();
	void OnMoveForward();
	void OnMoveBack();

	void OnCameraTurnLeft();
	void OnCameraTurnRight();
	void OnCameraZoomIn();

};