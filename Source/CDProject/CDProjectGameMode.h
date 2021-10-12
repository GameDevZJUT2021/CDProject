// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameInfo.h"
#include "CDProjectGameMode.generated.h"

UCLASS(minimalapi)
class ACDProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACDProjectGameMode();

	void StartPlay() override;

	AGameInfo* MyGameInfo;
};



