// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraDirector.generated.h"

UCLASS()
class CDPROJECT_API ACameraDirector : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 用于绑定功能和输入
	UPROPERTY(EditAnywhere)
		AActor* CameraOne;

	UPROPERTY(EditAnywhere)
		AActor* CameraTwo;

	UPROPERTY(EditAnywhere)
		AActor* CameraThree;
	float TimeToNextCameraChange;
	int levelSelect;
	int firstInit;
	void MoveLeft();
	void MoveRight();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
