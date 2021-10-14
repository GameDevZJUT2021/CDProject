#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "..\CommonEnum.h"
#include "ParentPawn.generated.h"

UCLASS()
class CDPROJECT_API AParentPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AParentPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocations);
	virtual void IndependentMove();
	virtual bool isMoveDone();

};
