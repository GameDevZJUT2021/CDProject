// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentPawn.h"
#include "ExampleEntityPawn.generated.h"

/*
  ������object is object��Чʱ�ṩת���Ļ�׼
 */
UCLASS()
class CDPROJECT_API AExampleEntityPawn : public APawn
{
	GENERATED_BODY()

public:
	AExampleEntityPawn();

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	bool bWalkable = true;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	EObjectTags Tag = EObjectTags::Baba;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	bool haveFace = 1;

	UPROPERTY(EditAnywhere, Category = "Animation")
	bool haveSkeletaAnimation = 0;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimationAsset* WalkAnim1;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimationAsset* WalkAnim2;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimationAsset* IdleAnim;

	UPROPERTY(EditAnywhere, Category = "Animation")
	bool isSelfRotating = 0;
	UPROPERTY(EditAnywhere, Category = "Animation")
	uint8 RotateSpeed = 0;
	
};
