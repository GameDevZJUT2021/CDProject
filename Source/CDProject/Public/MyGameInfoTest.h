// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameInfo.h"
#include "MyGameInfoTest.generated.h"

/**
 * 
 */

USTRUCT(Atomic)
struct FUnitInfoTest
{
	GENERATED_BODY()

	FUnitInfoTest() {};
	TArray<AParentPawn*> Objects;
	bool isEmpty() { return !Objects.Num(); }
};

UCLASS()
class CDPROJECT_API AMyGameInfoTest : public AMyGameInfo
{
	GENERATED_BODY()
	
};
