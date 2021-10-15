// Copyright Epic Games, Inc. All Rights Reserved.

#include "CDProjectGameMode.h"
#include "CDProjectPlayerController.h"
#include "EntityPawn.h"
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

	// StartPlay��������BeginPlay֮����õ�,��˿���������ɨ���ͼ���ɵ�ͼ��Ϣ

}