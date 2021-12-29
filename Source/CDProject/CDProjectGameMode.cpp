// Copyright Epic Games, Inc. All Rights Reserved.

#include "CDProjectGameMode.h"
#include "CDProjectPlayerController.h"
#include "EntityPawn.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"

ACDProjectGameMode::ACDProjectGameMode()
{
	// use our custom PlayerController class
	// PlayerControllerClass = ACDProjectPlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<ACDProjectPlayerController> ControllerClass(TEXT("/Game/BP/MyCDProjectPlayerController"));
	if (ControllerClass.Class != NULL)
	{
		PlayerControllerClass = ControllerClass.Class;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("No Controller")));
	}

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Game/BP/MyObservePawn"));
	if (PlayerPawnClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("No Pawn")));
	}
	
}

void ACDProjectGameMode::StartPlay() {
	Super::StartPlay();
	TActorIterator<AMyGameInfo> iter(GetWorld());
	checkf(iter, TEXT("There is no gameinfo"));
	AMyGameInfo* GameInfoToInit = *iter;
	GameInfoToInit->UpdateMapInfo();
	GameInfoToInit->UpdateRule(ECameraAbsLocations::South);
}