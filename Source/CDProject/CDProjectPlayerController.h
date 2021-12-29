// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ObservePawn.h"
#include "MyGameInfo.h"
#include "CommonEnum.h"
#include "CDProjectPlayerController.generated.h"

UCLASS()
class ACDProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACDProjectPlayerController();

protected:
	TQueue<EActions> ActionQueue;
	bool bOperatingAction = false;
	bool bWaiting = false;

	ECameraAbsLocations CameraAbsLocation = ECameraAbsLocations::South;

	AObservePawn* MyObservePawn;

	UPROPERTY(VisibleAnywhere)
	AMyGameInfo* MyGameInfo;

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void UpdateCameraAbsLocation(EActions Action);

	void ProcessMoveAction(EActions Action);
	bool ProcessMoveActionDone();

	void OnMoveLeft();
	void OnMoveRight();
	void OnMoveForward();
	void OnMoveBack();

	void OnWait();
	void OnFly();

	void OnCameraTurnLeft();
	void OnCameraTurnRight();
	//void OnCameraZoomIn();

};