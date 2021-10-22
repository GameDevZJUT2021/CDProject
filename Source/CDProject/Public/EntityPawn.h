// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentPawn.h"
#include "EntityPawn.generated.h"


UCLASS()
class CDPROJECT_API AEntityPawn : public AParentPawn
{
	GENERATED_BODY()

public:
	AEntityPawn();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	bool haveFace = 1;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual bool BeginControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) override;
	virtual bool BeginIndependentMove() override;
	virtual bool BeginMove(int AbsXdirection, int AbsYdirection) override;
	virtual bool isMoveDone() const override;

protected:
	const float Speed = 10.0f;
	FVector MoveDirection;
	FVector FaceDirection = FVector(0.0f, -1.0f, 0.0f);
	bool bMoving = false;
	FVector LocationBeforeMove;

};
