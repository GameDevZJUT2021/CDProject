 // Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"
#include "EntityPawn.h"
#include "RulePawn.h"

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

	// Test Rule
	//ActiveRules.Add(ERuleTags::You, ERuleTags::Baba);
	//ActiveRules.Add(ERuleTags::Push, ERuleTags::Water);

	RulesPool.Add(ERuleTags::You, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::You, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::You, ERuleTags::Water);
	RulesPool.Add(ERuleTags::Push, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::Push, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::Push, ERuleTags::Water);
	RulesPool.Add(ERuleTags::Win, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::Win, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::Win, ERuleTags::Water);
	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Water);
	RulesPool.Add(ERuleTags::Move, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::Move, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::Move, ERuleTags::Water);



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


TArray<EObjectTags> AGameInfo::GetObjectTags(ERuleTags ruleTag) const {
	TArray<ERuleTags> FindResultRuleTags;
	TArray<EObjectTags> objectTags;
	ActiveRules.MultiFind(ruleTag, FindResultRuleTags);

	for (ERuleTags tag : FindResultRuleTags)
	{
		int cast_value = static_cast<int>(tag);
		objectTags.Push(static_cast<EObjectTags>(cast_value));
	}

	return objectTags;
}
	

TArray<AEntityPawn*> AGameInfo::GetSelfPawns() const {
	TArray<AEntityPawn*> pSelfPawns;
	TArray<EObjectTags> objectTags;
	objectTags = GetObjectTags(ERuleTags::You);
	
	for (int i = 0; i < objectTags.Num(); i++)
	{
		for (TActorIterator<AEntityPawn> iter(GetWorld()); iter; ++iter)
		{
			AEntityPawn* pEntityPawn = *iter;
			if (pEntityPawn->Tag == objectTags[i])
				pSelfPawns.Push(pEntityPawn);
		}
	}

	return pSelfPawns;
}

TArray<AEntityPawn*> AGameInfo::GetMovePawns() const {
	TArray<AEntityPawn*> pMovePawns;
	TArray<EObjectTags> objectTags;
	objectTags = GetObjectTags(ERuleTags::Move);

	for (int i = 0; i < objectTags.Num(); i++)
	{
		for (TActorIterator<AEntityPawn> iter(GetWorld()); iter; ++iter)
		{
			AEntityPawn* pEntityPawn = *iter;
			if (pEntityPawn->Tag == objectTags[i])
				pMovePawns.Push(pEntityPawn);
		}
	}

	return pMovePawns;
}

void AGameInfo::UpdateMapInfo() {
	// 清空原地图信息
	for (int i = 0; i < MapLength * MapWidth; i++)
	{
		MapInfo[i].Objects.Empty();
	}

	// 更新现在地图信息
	for (TActorIterator<AParentPawn> iter(GetWorld()); iter; ++iter)
	{
		AParentPawn* ParentPawn = *iter;
		FVector PawnLocation = ParentPawn->GetActorLocation();

		checkf(static_cast<int>(PawnLocation.X) % 100 < 1 && static_cast<int>(PawnLocation.Y) % 100 < 1, TEXT("Location is not aligned"));
		int x = (PawnLocation.X + (MapWidth - 1) * 50) / 100;
		int y = (PawnLocation.Y + (MapLength - 1) * 50) / 100;
		checkf(0 <= x && x < MapWidth && 0 <= y && y < MapLength, TEXT("Location is out of the map"));

		MapInfo[x * MapWidth + y].Objects.Push(ParentPawn);
	}
}

bool AGameInfo::RuleIsVisible(ECameraAbsLocations CameraAbsLocation, const TArray<int>& MapInfo_X, const TArray<int>& MapInfo_Y, int CurrentIndex) const
{
	if (CameraAbsLocation == ECameraAbsLocations::South)
	{
		if (MapInfo_X[CurrentIndex] != 0)
		{
			for (int j = 0; j < MapInfo_X.Num();j++)
			{
				if (MapInfo_X[j] == MapInfo_X[CurrentIndex] - 1)
				{
					if (MapInfo_Y[j] == MapInfo_Y[CurrentIndex] - 1)
						return false;
					else if (MapInfo_Y[j] == MapInfo_Y[CurrentIndex])
						return false;
					else if (MapInfo_Y[j] == MapInfo_Y[CurrentIndex] + 1)
						return false;
				}
			}
		}
		return true;
	}
	else if (CameraAbsLocation == ECameraAbsLocations::North)
	{
		if (MapInfo_X[CurrentIndex] != MapWidth - 1)
		{
			for (int j = 0; j < MapInfo_X.Num();j++)
			{
				if (MapInfo_X[j] == MapInfo_X[CurrentIndex] + 1)
				{
					if (MapInfo_Y[j] == MapInfo_Y[CurrentIndex] - 1)
						return false;
					else if (MapInfo_Y[j] == MapInfo_Y[CurrentIndex])
						return false;
					else if (MapInfo_Y[j] == MapInfo_Y[CurrentIndex] + 1)
						return false;
				}
			}
		}
		return true;
	}
	else if (CameraAbsLocation == ECameraAbsLocations::West)
	{
		if (MapInfo_Y[CurrentIndex] != 0)
		{
			for (int j = 0; j < MapInfo_Y.Num();j++)
			{
				if (MapInfo_Y[j] == MapInfo_Y[CurrentIndex] - 1)
				{
					if (MapInfo_X[j] == MapInfo_X[CurrentIndex] - 1)
						return false;
					else if (MapInfo_X[j] == MapInfo_X[CurrentIndex])
						return false;
					else if (MapInfo_X[j] == MapInfo_X[CurrentIndex] + 1)
						return false;
				}
			}
		}
		return true;
	}
	else if (CameraAbsLocation == ECameraAbsLocations::East)
	{
		if (MapInfo_Y[CurrentIndex] != MapLength - 1)
		{
			for (int j = 0; j < MapInfo_Y.Num();j++)
			{
				if (MapInfo_Y[j] == MapInfo_Y[CurrentIndex] + 1)
				{
					if (MapInfo_X[j] == MapInfo_X[CurrentIndex] - 1)
						return false;
					else if (MapInfo_X[j] == MapInfo_X[CurrentIndex])
						return false;
					else if (MapInfo_X[j] == MapInfo_X[CurrentIndex] + 1)
						return false;
				}
			}
		}
		return true;
	}
	return false;
}

void AGameInfo::UpdateRule(ECameraAbsLocations CameraAbsLocation) {
	ActiveRules.Empty();
	TArray<int> MapInfo_X, MapInfo_Y;
	TArray<ARulePawn*> RulePawnArray;

	// save all rule pawns' infomation
	for (TActorIterator<ARulePawn> iter(GetWorld()); iter; ++iter)
	{
		ARulePawn* rulePawn = *iter;
		FVector PawnLocation = rulePawn->GetActorLocation();
		int x = (PawnLocation.X + (MapWidth - 1) * 50) / 100;
		int y = (PawnLocation.Y + (MapLength - 1) * 50) / 100;
		checkf(0 <= x && x < MapWidth && 0 <= y && y < MapLength, TEXT("Location Wrong"));
		MapInfo_X.Push(x);
		MapInfo_Y.Push(y);
		RulePawnArray.Push(rulePawn);
	}


	if (CameraAbsLocation == ECameraAbsLocations::South || CameraAbsLocation == ECameraAbsLocations::North)
	{
		for (int i = 0; i < RulePawnArray.Num(); i++)
		{
			// 南北方向
			// 边界检查,舍去最外围的方块
			if (RulePawnArray[i]->TopTag == ERuleTags::is)
			{
				if (MapInfo_X[i] != 0 && MapInfo_X[i] != MapWidth - 1)
				{
					bool bSouthExist = 0, bNorthExist = 0;
					ARulePawn* VerticalFirstRulePawn = nullptr, * VerticalLastRulePawn = nullptr;
					for (int j = 0; j < MapInfo_X.Num(); j++)
					{
						if (MapInfo_X[j] == MapInfo_X[i] - 1)
						{
							if (MapInfo_Y[j] == MapInfo_Y[i])
							{
								bSouthExist = 1;
								VerticalLastRulePawn = RulePawnArray[j];
							}
						}
						else if (MapInfo_X[j] == MapInfo_X[i] + 1)
						{
							if (MapInfo_Y[j] == MapInfo_Y[i])
							{
								bNorthExist = 1;
								VerticalFirstRulePawn = RulePawnArray[j];
							}
						}
						if (bSouthExist && bNorthExist) // 规则形成
						{
							// 检查规则是否合理 并加入规则
							// 规则是倒着存储的
							if (RulesPool.FindPair(VerticalLastRulePawn->TopTag, VerticalFirstRulePawn->TopTag))
								ActiveRules.Add(VerticalLastRulePawn->TopTag, VerticalFirstRulePawn->TopTag);
							break;
						}
					}
				}
			}

			// 东西方向
			if (MapInfo_Y[i] != 0 && MapInfo_Y[i] != MapLength - 1)
			{
				bool bWestExist = 0, bEastExist = 0;
				ARulePawn* HorizontalFirstRulePawn = nullptr, * HorizontalLastRulePawn = nullptr;
				for (int j = 0; j < MapInfo_Y.Num(); j++)
				{
					if (MapInfo_Y[j] == MapInfo_Y[i] - 1)
					{
						if (MapInfo_X[j] == MapInfo_X[i])
						{
							bWestExist = 1;
							HorizontalFirstRulePawn = RulePawnArray[j];
						}
					}
					else if (MapInfo_Y[j] == MapInfo_Y[i] + 1)
					{
						if (MapInfo_X[j] == MapInfo_X[i])
						{
							bEastExist = 1;
							HorizontalLastRulePawn = RulePawnArray[j];
						}
					}
					if (bWestExist && bEastExist)
					{
						// 检测并加入top面规则
						if (RulePawnArray[i]->TopTag == ERuleTags::is)
						{
							if (RulesPool.FindPair(HorizontalLastRulePawn->TopTag, HorizontalFirstRulePawn->TopTag))
								ActiveRules.Add(HorizontalLastRulePawn->TopTag, HorizontalFirstRulePawn->TopTag);
						}
						if (CameraAbsLocation == ECameraAbsLocations::South) {
							if (RulePawnArray[i]->SouthTag == ERuleTags::is) {
								// 检测south面规则是否被遮挡
								if (RuleIsVisible(ECameraAbsLocations::South, MapInfo_X, MapInfo_Y, i))
								{
									if (RulesPool.FindPair(HorizontalLastRulePawn->SouthTag, HorizontalFirstRulePawn->SouthTag))
										ActiveRules.Add(HorizontalLastRulePawn->SouthTag, HorizontalFirstRulePawn->SouthTag);
								}
							}
						}
						else if (CameraAbsLocation == ECameraAbsLocations::North) {
							if (RulePawnArray[i]->NorthTag == ERuleTags::is) {
								//检测north面规则是否被遮挡
								if (RuleIsVisible(ECameraAbsLocations::North, MapInfo_X, MapInfo_Y, i))
								{
									if (RulesPool.FindPair(HorizontalFirstRulePawn->NorthTag, HorizontalLastRulePawn->NorthTag))
										ActiveRules.Add(HorizontalFirstRulePawn->NorthTag, HorizontalLastRulePawn->NorthTag);
								}
							}
						}
						break;
					}
				}
			}
		}
	}
	else if (CameraAbsLocation == ECameraAbsLocations::West || CameraAbsLocation == ECameraAbsLocations::East)
	{
		for (int i = 0; i < RulePawnArray.Num(); i++)
		{
			// 东西方向
			// 边界检查,舍去最外围的方块
			if (RulePawnArray[i]->TopTag == ERuleTags::is)
			{
				if (MapInfo_Y[i] != 0 && MapInfo_Y[i] != MapLength - 1)
				{
					bool bEastExist = 0, bWestExist = 0;
					ARulePawn* VerticalFirstRulePawn = nullptr, * VerticalLastRulePawn = nullptr;
					for (int j = 0; j < MapInfo_Y.Num(); j++)
					{
						if (MapInfo_Y[j] == MapInfo_Y[i] - 1)
						{
							if (MapInfo_X[j] == MapInfo_X[i])
							{
								bWestExist = 1;
								VerticalFirstRulePawn = RulePawnArray[j];
							}
						}
						else if (MapInfo_Y[j] == MapInfo_Y[i] + 1)
						{
							if (MapInfo_X[j] == MapInfo_X[i])
							{
								bEastExist = 1;
								VerticalLastRulePawn = RulePawnArray[j];
							}
						}
						if (bWestExist && bEastExist)
						{
							if (RulesPool.FindPair(VerticalLastRulePawn->TopTag, VerticalFirstRulePawn->TopTag))
								ActiveRules.Add(VerticalLastRulePawn->TopTag, VerticalFirstRulePawn->TopTag);
							break;
						}
					}
				}
			}

			// 南北方向
			if (MapInfo_X[i] != 0 && MapInfo_X[i] != MapWidth - 1)
			{
				bool bSouthExist = 0, bNorthExist = 0;
				ARulePawn* HorizontalFirstRulePawn = nullptr, * HorizontalLastRulePawn = nullptr;
				for (int j = 0; j < MapInfo_X.Num(); j++)
				{
					if (MapInfo_X[j] == MapInfo_X[i] - 1)
					{
						if (MapInfo_Y[j] == MapInfo_Y[i])
						{
							bSouthExist = 1;
							HorizontalLastRulePawn = RulePawnArray[j];
						}
					}
					else if (MapInfo_X[j] == MapInfo_X[i] + 1)
					{
						if (MapInfo_Y[j] == MapInfo_Y[i])
						{
							bNorthExist = 1;
							HorizontalFirstRulePawn = RulePawnArray[j];
						}
					}
					if (bSouthExist && bNorthExist)
					{
						if (RulePawnArray[i]->TopTag == ERuleTags::is)
						{
							if (RulesPool.FindPair(HorizontalLastRulePawn->TopTag, HorizontalFirstRulePawn->TopTag))
								ActiveRules.Add(HorizontalLastRulePawn->TopTag, HorizontalFirstRulePawn->TopTag);
						}
						if (CameraAbsLocation == ECameraAbsLocations::West)
						{
							if (RulePawnArray[i]->WestTag == ERuleTags::is)
							{
								//检测West面规则是否被遮挡
								if (RuleIsVisible(ECameraAbsLocations::West, MapInfo_X, MapInfo_Y, i))
								{
									if (RulesPool.FindPair(HorizontalLastRulePawn->WestTag, HorizontalFirstRulePawn->WestTag))
										ActiveRules.Add(HorizontalLastRulePawn->WestTag, HorizontalFirstRulePawn->WestTag);
								}
							}
						}
						else if (CameraAbsLocation == ECameraAbsLocations::East)
						{
							if (RulePawnArray[i]->EastTag == ERuleTags::is)
							{
								//检测East面规则是否被遮挡
								if (RuleIsVisible(ECameraAbsLocations::East, MapInfo_X, MapInfo_Y, i))
									if (RulesPool.FindPair(HorizontalFirstRulePawn->EastTag, HorizontalLastRulePawn->EastTag))
										ActiveRules.Add(HorizontalFirstRulePawn->EastTag, HorizontalLastRulePawn->EastTag);
							}
						}
						break;
					}
				}
			}
		}
	}
}

bool AGameInfo::WinJudge() const{
	TArray<EObjectTags> WinPawnTags = GetObjectTags(ERuleTags::Win);
	TArray<EObjectTags> YouPawnTags = GetObjectTags(ERuleTags::You);


	for (UnitInfo unitInfo : MapInfo)
	{
		bool bWin = false, bYou = false;
		for (AParentPawn* pPawn : unitInfo.Objects)
		{
			if (WinPawnTags.Find(pPawn->Tag) != INDEX_NONE)
				bWin = true;
			if (YouPawnTags.Find(pPawn->Tag) != INDEX_NONE)
				bYou = true;
 		}
		if (bWin && bYou)
			return true;
	}

	return false;
}

bool AGameInfo::DefeatJudge() const{
	TArray<EObjectTags> DefeatPawnTags = GetObjectTags(ERuleTags::Defeat);
	TArray<EObjectTags> YouPawnTags = GetObjectTags(ERuleTags::You);


	for (UnitInfo unitInfo : MapInfo)
	{
		bool bDefeat = false, bYou = false;
		for (AParentPawn* pPawn : unitInfo.Objects)
		{
			if (DefeatPawnTags.Find(pPawn->Tag) != INDEX_NONE)
				bDefeat = true;
			if (YouPawnTags.Find(pPawn->Tag) != INDEX_NONE)
				bYou = true;
		}
		if (bDefeat && bYou)
			return true;
	}

	return false;
}

