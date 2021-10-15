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
	//RulesPool.Init("", 1);

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

// 未实现,只简单返回Baba作为selftag,应该通过读取ActiveRules来获取SelfTags
TArray<EObjectTags> AGameInfo::GetSelfTags() const{
	TArray<EObjectTags> SelfTags;
	SelfTags.Init(EObjectTags::Baba,1);
	return SelfTags;
}

// 未实现
TArray<AParentPawn*> AGameInfo::GetSelfPawns() const {
	TArray<AParentPawn*> SelfPawns;
	SelfPawns.Init(nullptr, 1);
	return SelfPawns;

	//TArray<EObjectTags> SelfTags = GetSelfTags();
	//for (int i = 0; i < MapWidth; i++)
	//	for (int j = 0; j < MapLength; j++)
	//		for (const EObjectTags& SelfTag : SelfTags)
	//		{
	//			check if MapInfo[i * MapWidth + j] == SelfTag;
	//			add into array
	//		}
}

void AGameInfo::Update() {

}

