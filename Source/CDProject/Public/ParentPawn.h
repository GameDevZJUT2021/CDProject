#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EngineUtils.h"
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

	UPROPERTY(EditAnywhere)
	bool bWalkable = true;

	UPROPERTY(EditAnywhere)
	EObjectTags Tag = EObjectTags::Baba;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// return true if this pawn could move to destination
	virtual bool BeginControlledMove(EActions Action, ECameraAbsLocations CameraAbsLocation);
	virtual bool BeginIndependentMove();
	virtual bool BeginMove(int AbsXdirection, int AbsYdirection, bool ControlledOrIndenpent = 1);
	virtual bool isMoveDone() const;

};
