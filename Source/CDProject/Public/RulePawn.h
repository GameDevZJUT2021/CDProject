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
	UPROPERTY(VisibleAnywhere, Category = "Rule")
	bool TopTagActive = false;// used to control the light effect of rule

	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags BottomTag;
	UPROPERTY(VisibleAnywhere, Category = "Rule")
	bool BottomTagActive = false;

	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags SouthTag;
	UPROPERTY(VisibleAnywhere, Category = "Rule")
	bool SouthTagActive = false;

	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags WestTag;
	UPROPERTY(VisibleAnywhere, Category = "Rule")
	bool WestTagActive = false;

	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags NorthTag;
	UPROPERTY(VisibleAnywhere, Category = "Rule")
	bool NorthTagActive = false;

	UPROPERTY(EditAnywhere, Category = "Rule")
	ERuleTags EastTag;
	UPROPERTY(VisibleAnywhere, Category = "Rule")
	bool EastTagActive = false;

	UPROPERTY(EditAnywhere, Category = "RuleMaterial")
	UMaterialInterface* TopMaterial; // used to generate dynamic material
	UPROPERTY(VisibleAnywhere, Category = "RuleMaterial")
	UMaterialInterface* BottomMaterial; // just used to match the material's order in StaticMesh 
	UPROPERTY(EditAnywhere, Category = "RuleMaterial")
	UMaterialInterface* EastMaterial;
	UPROPERTY(EditAnywhere, Category = "RuleMaterial")
	UMaterialInterface* WestMaterial;
	UPROPERTY(EditAnywhere, Category = "RuleMaterial")
	UMaterialInterface* NorthMaterial;
	UPROPERTY(EditAnywhere, Category = "RuleMaterial")
	UMaterialInterface* SouthMaterial;


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
