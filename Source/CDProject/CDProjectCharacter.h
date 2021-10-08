// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CDProjectCharacter.generated.h"

UCLASS(Blueprintable)
class ACDProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACDProjectCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	void MoveForward(float Value);
	void MoveBack(float Value);
	void MoveLeft(float Value);
	void MoveRight(float Value);


private:
	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
};

