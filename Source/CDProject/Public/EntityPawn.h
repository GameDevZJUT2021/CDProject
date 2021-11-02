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
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	bool haveFace = 1;

	UPROPERTY(EditAnywhere)
	bool haveAnimation = 0;

	// Ñ­»·²¥·ÅWalkAnim1ºÍWalkAnim2
	UPROPERTY(EditAnywhere)
	UAnimationAsset* WalkAnim1;
	UPROPERTY(EditAnywhere)
	UAnimationAsset* WalkAnim2;
	UPROPERTY(EditAnywhere)
	UAnimationAsset* IdleAnim;

	UPROPERTY(EditAnywhere)
	FVector FaceDirection = FVector(0.0f, -1.0f, 0.0f);

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual bool BeginControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) override;
	virtual bool BeginIndependentMove() override;
	virtual bool BeginMove(int AbsXdirection, int AbsYdirection, bool ControlledOrIndenpent =1) override;
	virtual bool isMoveDone() const override;

protected:
	const float Speed = 10.0f;
	FVector MoveDirection;
	bool bMoving = false;
	FVector LocationBeforeMove;

};
