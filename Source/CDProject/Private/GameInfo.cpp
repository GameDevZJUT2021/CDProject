 // Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"

// Sets default values
AGameInfo::AGameInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Init the MapInfo
void AGameInfo::Init(int width, int length)
{
	MapWidth = width;
	MapLength = length;
	MapInfo.Init(UnitInfo(), width * length);
	RulesPool.Init("", 1);

	//scan the map to fill in the MapInfo and ActiveRules?
}

// Called when the game starts or when spawned
void AGameInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

