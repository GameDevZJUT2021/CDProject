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

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags TopTag;
	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags BottomTag;
	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags SouthTag;
	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags WestTag;
	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags NorthTag;
	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags EastTag;


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual bool BeginControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) override;
	virtual bool BeginIndependentMove() override;
	virtual bool BeginMove(int AbsXdirection, int AbsYdirection, bool ControlledOrIndenpent = 1) override;
	virtual bool isMoveDone() const override;

protected:
	const float Speed = 10.0f;
	FVector MoveDirection;
	bool bMoving = false;
	FVector LocationBeforeMove;
	
};
