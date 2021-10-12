// Copyright Epic Games, Inc. All Rights Reserved.

#include "CDProjectGameMode.h"
#include "CDProjectPlayerController.h"
#include "CDProjectCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"

ACDProjectGameMode::ACDProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACDProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

void ACDProjectGameMode::StartPlay() {
	Super::StartPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		MyGameInfo = World->SpawnActor<AGameInfo>();
		MyGameInfo->Init(25, 25);
	}
	//for (TObjectIterator<AStaticMeshActor> Itr; Itr; ++Itr)
	//{
	//	AStaticMeshActor* curr = *Itr;
	//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, curr->GetHumanReadableName());
	//}
}