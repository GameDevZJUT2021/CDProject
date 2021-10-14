// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CommonEnum.h"
#include "ObservePawn.generated.h"

UCLASS()
class CDPROJECT_API AObservePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AObservePawn();

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	FVector RotateAxis = FVector(0.0f, 0.0f, 1.0f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bRotating = false;
	const int RotateSpeed = 3;
	int RotateDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void BeginRotate(EActions Action);

	bool isRotateDone();

};
