// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentPawn.h"
#include "Components/BoxComponent.h"
#include "CubePawn.generated.h"

/**
 * 
 */
UCLASS()
class CDPROJECT_API ACubePawn : public AParentPawn
{
	GENERATED_BODY()

public:
	ACubePawn();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const float Speed = 10.0f;
	FVector Direction;
	bool bMoving = false;
	void ControlMove(int Action, int CameraDirection) override;
	bool isMoveDone() override;

private:
	FVector LocationBeforeMove;
	
};


