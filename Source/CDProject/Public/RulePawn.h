// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentPawn.h"
#include "RulePawn.generated.h"

/**
 * 
 */
UCLASS()
class CDPROJECT_API ARulePawn : public AParentPawn
{
	GENERATED_BODY()

public:
	ARulePawn();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere)
	ERuleTags TopTag;
	UPROPERTY(EditAnywhere)
	ERuleTags BottomTag;
	UPROPERTY(EditAnywhere)
	ERuleTags SouthTag;
	UPROPERTY(EditAnywhere)
	ERuleTags WestTag;
	UPROPERTY(EditAnywhere)
	ERuleTags NorthTag;
	UPROPERTY(EditAnywhere)
	ERuleTags EastTag;


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual bool BeginControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) override;
	virtual bool BeginIndependentMove() override;
	virtual bool BeginMove(int AbsXdirection, int AbsYdirection) override;
	virtual bool isMoveDone() const override;

protected:
	const float Speed = 10.0f;
	FVector MoveDirection;
	bool bMoving = false;
	FVector LocationBeforeMove;
	
};
