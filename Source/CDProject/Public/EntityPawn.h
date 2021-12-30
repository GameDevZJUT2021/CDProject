// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "ParentPawn.h"
#include "Components/AudioComponent.h"
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

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	bool haveFace = 1;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	FVector FaceDirection = FVector(0.0f, -1.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Animation")
	bool haveSkeletaAnimation = 0;
	// ѭ������WalkAnim1��WalkAnim2��ģ�����ҽ���·
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimationAsset* WalkAnim1;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimationAsset* WalkAnim2;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimationAsset* IdleAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimationAsset* SlideBeginAnim;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimationAsset* SlideEndAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	bool isSelfRotating = 0;
	UPROPERTY(EditAnywhere, Category = "Animation")
	uint8 RotateSpeed = 0;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* WalkSound;
	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* SlideSound;

	UAudioComponent* SlideAudioComp = nullptr;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool BeginControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation) override;
	virtual bool BeginIndependentMove() override;
	virtual bool BeginMove(int AbsXdirection, int AbsYdirection, bool ControlledOrIndenpent =1) override;
	virtual bool isMoveDone() const override;

	void FallingDown() override;

	void BeginFlyOrFall();

protected:
	const float Speed = 10.0f;
	FVector MoveDirection;
	bool bMoving = false;
	FVector LocationBeforeMove;

	bool bSliding = false;
	FVector SlidingDestination;
	const float SlideSpeed = 20.0f;


	bool bFlyMoving = false;//�Ƿ������������½�
	const float FlySpeed = 10.0f;
	bool bFlying = false;//��������
	bool bFalling = false;//�����½�
	bool bFloating = false;//��Ư���ڿ���,������������ý�ɫ�ڿ������¸���
};
