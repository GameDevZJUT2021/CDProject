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

	const float Speed = 10.0f;
	FVector MoveDirection;
	FVector FaceDirecion;
	bool bMoving = false;

	virtual bool ControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) override;
	virtual void IndependentMove() override;
	virtual bool isMoveDone() override;

private:
	FVector LocationBeforeMove;
	
};
