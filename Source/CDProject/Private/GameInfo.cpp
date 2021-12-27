 // Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"
#include "EntityPawn.h"
#include "RulePawn.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AGameInfo::AGameInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

}

// Init the MapInfo
void AGameInfo::Init(int width, int length)
{
	MapWidth = width;
	MapLength = length;
	MapInfo.Init(UnitInfo(), width * length);

	RulesPool.Add(ERuleTags::You, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::You, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::You, ERuleTags::Water);
	RulesPool.Add(ERuleTags::You, ERuleTags::Bush);
	RulesPool.Add(ERuleTags::You, ERuleTags::Gold);
	RulesPool.Add(ERuleTags::You, ERuleTags::Trap);
	RulesPool.Add(ERuleTags::You, ERuleTags::Box);

	RulesPool.Add(ERuleTags::Push, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::Push, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::Push, ERuleTags::Water);
	RulesPool.Add(ERuleTags::Push, ERuleTags::Bush);
	RulesPool.Add(ERuleTags::Push, ERuleTags::Gold);
	RulesPool.Add(ERuleTags::Push, ERuleTags::Trap);
	RulesPool.Add(ERuleTags::Push, ERuleTags::Box);

	RulesPool.Add(ERuleTags::Win, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::Win, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::Win, ERuleTags::Water);
	RulesPool.Add(ERuleTags::Win, ERuleTags::Bush);
	RulesPool.Add(ERuleTags::Win, ERuleTags::Gold);
	RulesPool.Add(ERuleTags::Win, ERuleTags::Trap);
	RulesPool.Add(ERuleTags::Win, ERuleTags::Box);

	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Water);
	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Bush);
	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Gold);
	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Trap);
	RulesPool.Add(ERuleTags::Defeat, ERuleTags::Box);

	RulesPool.Add(ERuleTags::Move, ERuleTags::Baba);
	RulesPool.Add(ERuleTags::Move, ERuleTags::Tree);
	RulesPool.Add(ERuleTags::Move, ERuleTags::Water);
	RulesPool.Add(ERuleTags::Move, ERuleTags::Bush);
	RulesPool.Add(ERuleTags::Move, ERuleTags::Gold);
	RulesPool.Add(ERuleTags::Move, ERuleTags::Trap);
	RulesPool.Add(ERuleTags::Move, ERuleTags::Box);

}

 //Called when the game starts or when spawned
void AGameInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

 //Called every frame
void AGameInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


TArray<EObjectTags> AGameInfo::GetObjectTags(ERuleTags ruleTag) const {
	if (!isAttribute(ruleTag))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,TEXT("Get Object Tags Error: No such Rule"));
		return TArray<EObjectTags>();
	}

	TArray<ERuleTags> FindResultRuleTags;
	TArray<EObjectTags> objectTags;
	ActiveRules.MultiFind(ruleTag, FindResultRuleTags);

	for (ERuleTags tag : FindResultRuleTags)
		objectTags.Push(static_cast<EObjectTags>(tag));

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
		AParentPawn* pParentPawn = *iter;
		FVector PawnLocation = pParentPawn->GetActorLocation();

		// 忽略位置错误的pawn
		if (static_cast<int>(PawnLocation.X) % 100 > 0.01 && static_cast<int>(PawnLocation.Y) % 100 > 0.01)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,TEXT("Location is not aligned, Some errors may occured"));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,pParentPawn->GetHumanReadableName());
			continue;
		}
		int x = (PawnLocation.X + (MapWidth - 1) * 50) / 100;
		int y = (PawnLocation.Y + (MapLength - 1) * 50) / 100;
		// 忽略位置错误的pawn
		if (0 > x || x >= MapWidth || 0 > y || y >= MapLength)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Location is out of the map, Some errors may occured"));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, pParentPawn->GetHumanReadableName());
			continue;
		}

		MapInfo[x * MapWidth + y].Objects.Push(pParentPawn);
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
	TArray<ARulePawn*> pRulePawnArray;

	// save all rule pawns' infomation
	for (TActorIterator<ARulePawn> iter(GetWorld()); iter; ++iter)
	{
		ARulePawn* pRulePawn = *iter;
		FVector PawnLocation = pRulePawn->GetActorLocation();
		int x = (PawnLocation.X + (MapWidth - 1) * 50) / 100;
		int y = (PawnLocation.Y + (MapLength - 1) * 50) / 100;
		checkf(0 <= x && x < MapWidth && 0 <= y && y < MapLength, TEXT("Location Wrong"));
		MapInfo_X.Push(x);
		MapInfo_Y.Push(y);
		pRulePawn->TopTagActive = false;
		pRulePawn->EastTagActive = false;
		pRulePawn->WestTagActive = false;
		pRulePawn->NorthTagActive = false;
		pRulePawn->SouthTagActive = false;
		pRulePawnArray.Push(pRulePawn);
	}


	if (CameraAbsLocation == ECameraAbsLocations::South || CameraAbsLocation == ECameraAbsLocations::North)
	{
		for (int i = 0; i < pRulePawnArray.Num(); i++)
		{
			// 规则方块上面的南北方向规则判断
			// 边界检查,舍去最外围的方块
			if (pRulePawnArray[i]->TopTag == ERuleTags::is)
			{
				if (MapInfo_X[i] != 0 && MapInfo_X[i] != MapWidth - 1)
				{
					bool bSouthExist = 0, bNorthExist = 0;
					ARulePawn* pVerticalFirstRulePawn = nullptr, * pVerticalLastRulePawn = nullptr;
					for (int j = 0; j < MapInfo_X.Num(); j++)
					{
						if (MapInfo_X[j] == MapInfo_X[i] - 1)
						{
							if (MapInfo_Y[j] == MapInfo_Y[i])
							{
								bSouthExist = 1;
								pVerticalLastRulePawn = pRulePawnArray[j];
							}
						}
						else if (MapInfo_X[j] == MapInfo_X[i] + 1)
						{
							if (MapInfo_Y[j] == MapInfo_Y[i])
							{
								bNorthExist = 1;
								pVerticalFirstRulePawn = pRulePawnArray[j];
							}
						}
						if (bSouthExist && bNorthExist) // 规则形成
						{
							TryToActivateRule(pVerticalFirstRulePawn, pVerticalLastRulePawn, pRulePawnArray[i], Face::Top);
							break;
						}
					}
				}
			}
			// 东西方向
			// 先找到3个连在一起的再判断中间是否为is, 这样可以一次判断两个面
			if (MapInfo_Y[i] != 0 && MapInfo_Y[i] != MapLength - 1)
			{
				bool bWestExist = 0, bEastExist = 0;
				ARulePawn* pHorizontalFirstRulePawn = nullptr, * pHorizontalLastRulePawn = nullptr;
				for (int j = 0; j < MapInfo_Y.Num(); j++)
				{
					if (MapInfo_Y[j] == MapInfo_Y[i] - 1)
					{
						if (MapInfo_X[j] == MapInfo_X[i])
						{
							bWestExist = 1;
							pHorizontalFirstRulePawn = pRulePawnArray[j];
						}
					}
					else if (MapInfo_Y[j] == MapInfo_Y[i] + 1)
					{
						if (MapInfo_X[j] == MapInfo_X[i])
						{
							bEastExist = 1;
							pHorizontalLastRulePawn = pRulePawnArray[j];
						}
					}
					if (bWestExist && bEastExist)
					{
						// Top face
						if (pRulePawnArray[i]->TopTag == ERuleTags::is)
							TryToActivateRule(pHorizontalFirstRulePawn, pHorizontalLastRulePawn, pRulePawnArray[i], Face::Top);

						if (CameraAbsLocation == ECameraAbsLocations::South)
						{
							// 检测south面规则是否被遮挡
							if (pRulePawnArray[i]->SouthTag == ERuleTags::is && RuleIsVisible(ECameraAbsLocations::South, MapInfo_X, MapInfo_Y, i))
								TryToActivateRule(pHorizontalFirstRulePawn, pHorizontalLastRulePawn, pRulePawnArray[i], Face::South);
						}
						else if (CameraAbsLocation == ECameraAbsLocations::North) 
						{
							//检测north面规则是否被遮挡
							if (pRulePawnArray[i]->NorthTag == ERuleTags::is && RuleIsVisible(ECameraAbsLocations::North, MapInfo_X, MapInfo_Y, i)) 
								TryToActivateRule(pHorizontalFirstRulePawn, pHorizontalLastRulePawn, pRulePawnArray[i], Face::North);
						}
						break;
					}
				}
			}
		}
	}

	else if (CameraAbsLocation == ECameraAbsLocations::West || CameraAbsLocation == ECameraAbsLocations::East)
	{
		for (int i = 0; i < pRulePawnArray.Num(); i++)
		{
			// 东西方向
			// 边界检查,舍去最外围的方块
			if (pRulePawnArray[i]->TopTag == ERuleTags::is)
			{
				if (MapInfo_Y[i] != 0 && MapInfo_Y[i] != MapLength - 1)
				{
					bool bEastExist = 0, bWestExist = 0;
					ARulePawn* pVerticalFirstRulePawn = nullptr, * pVerticalLastRulePawn = nullptr;
					for (int j = 0; j < MapInfo_Y.Num(); j++)
					{
						if (MapInfo_Y[j] == MapInfo_Y[i] - 1)
						{
							if (MapInfo_X[j] == MapInfo_X[i])
							{
								bWestExist = 1;
								pVerticalFirstRulePawn = pRulePawnArray[j];
							}
						}
						else if (MapInfo_Y[j] == MapInfo_Y[i] + 1)
						{
							if (MapInfo_X[j] == MapInfo_X[i])
							{
								bEastExist = 1;
								pVerticalLastRulePawn = pRulePawnArray[j];
							}
						}
						if (bWestExist && bEastExist)
						{
							TryToActivateRule(pVerticalFirstRulePawn, pVerticalLastRulePawn, pRulePawnArray[i], Face::Top);
							break;
						}
					}
				}
			}

			// 南北方向
			if (MapInfo_X[i] != 0 && MapInfo_X[i] != MapWidth - 1)
			{
				bool bSouthExist = 0, bNorthExist = 0;
				ARulePawn* pHorizontalFirstRulePawn = nullptr, * pHorizontalLastRulePawn = nullptr;
				for (int j = 0; j < MapInfo_X.Num(); j++)
				{
					if (MapInfo_X[j] == MapInfo_X[i] - 1)
					{
						if (MapInfo_Y[j] == MapInfo_Y[i])
						{
							bSouthExist = 1;
							pHorizontalLastRulePawn = pRulePawnArray[j];
						}
					}
					else if (MapInfo_X[j] == MapInfo_X[i] + 1)
					{
						if (MapInfo_Y[j] == MapInfo_Y[i])
						{
							bNorthExist = 1;
							pHorizontalFirstRulePawn = pRulePawnArray[j];
						}
					}
					if (bSouthExist && bNorthExist)
					{
						if (pRulePawnArray[i]->TopTag == ERuleTags::is)
							TryToActivateRule(pHorizontalFirstRulePawn, pHorizontalLastRulePawn, pRulePawnArray[i], Face::Top);

						if (CameraAbsLocation == ECameraAbsLocations::West)
						{
							//检测West面规则是否被遮挡
							if (pRulePawnArray[i]->WestTag == ERuleTags::is && RuleIsVisible(ECameraAbsLocations::West, MapInfo_X, MapInfo_Y, i))
								TryToActivateRule(pHorizontalFirstRulePawn, pHorizontalLastRulePawn, pRulePawnArray[i], Face::West);
						}
						else if (CameraAbsLocation == ECameraAbsLocations::East)
						{
							//检测East面规则是否被遮挡
							if (pRulePawnArray[i]->EastTag == ERuleTags::is && RuleIsVisible(ECameraAbsLocations::East, MapInfo_X, MapInfo_Y, i))
								TryToActivateRule(pHorizontalFirstRulePawn, pHorizontalLastRulePawn, pRulePawnArray[i], Face::East);
						}
						break;
					}
				}
			}
		}
	}
	UpdateLightEffect();
}

void AGameInfo::TryToActivateRule(ARulePawn* pFirstRulePawn, ARulePawn* pLastRulePawn, ARulePawn* pIsRulePawn, enum Face face) {
	
	switch (face)
	{
	case Top:
		// object is attribute
		if (isObject(pFirstRulePawn->TopTag) && isAttribute(pLastRulePawn->TopTag))
		{
			ActiveRules.Add(pLastRulePawn->TopTag, pFirstRulePawn->TopTag);
			// turn on the light effect in UpdateLightEffect function
			pLastRulePawn->TopTagActive = true;
			pFirstRulePawn->TopTagActive = true;
			pIsRulePawn->TopTagActive = true;
		}
		// object a is object b, change all object a to object b
		else if (isObject(pFirstRulePawn->TopTag) && isObject(pLastRulePawn->TopTag))
		{
			ChangeAllEntity(static_cast<EObjectTags>(pFirstRulePawn->TopTag), static_cast<EObjectTags>(pLastRulePawn->TopTag));
		}
		break;

	case South:
		// object is attribute
		if (isObject(pFirstRulePawn->SouthTag) && isAttribute(pLastRulePawn->SouthTag))
		{
			ActiveRules.Add(pLastRulePawn->SouthTag, pFirstRulePawn->SouthTag);
			// turn on the light effect in UpdateLightEffect function
			pLastRulePawn->SouthTagActive = true;
			pFirstRulePawn->SouthTagActive = true;
			pIsRulePawn->SouthTagActive = true;
		}
		// object a is object b, change all object a to object b
		else if (isObject(pFirstRulePawn->SouthTag) && isObject(pLastRulePawn->SouthTag))
		{
			ChangeAllEntity(static_cast<EObjectTags>(pFirstRulePawn->SouthTag), static_cast<EObjectTags>(pLastRulePawn->SouthTag));
		}
		break;

	case West:
		// object is attribute
		if (isObject(pFirstRulePawn->WestTag) && isAttribute(pLastRulePawn->WestTag))
		{
			ActiveRules.Add(pLastRulePawn->WestTag, pFirstRulePawn->WestTag);
			// turn on the light effect in UpdateLightEffect function
			pLastRulePawn->WestTagActive = true;
			pFirstRulePawn->WestTagActive = true;
			pIsRulePawn->WestTagActive = true;
		}
		// object a is object b, change all object a to object b
		else if (isObject(pFirstRulePawn->WestTag) && isObject(pLastRulePawn->WestTag))
		{
			ChangeAllEntity(static_cast<EObjectTags>(pFirstRulePawn->WestTag), static_cast<EObjectTags>(pLastRulePawn->WestTag));
		}
		break;

	case East:
		// object is attribute
		if (isObject(pFirstRulePawn->EastTag) && isAttribute(pLastRulePawn->EastTag))
		{
			ActiveRules.Add(pLastRulePawn->EastTag, pFirstRulePawn->EastTag);
			// turn on the light effect in UpdateLightEffect function
			pLastRulePawn->EastTagActive = true;
			pFirstRulePawn->EastTagActive = true;
			pIsRulePawn->EastTagActive = true;
		}
		// object a is object b, change all object a to object b
		else if (isObject(pFirstRulePawn->EastTag) && isObject(pLastRulePawn->EastTag))
		{
			ChangeAllEntity(static_cast<EObjectTags>(pFirstRulePawn->EastTag), static_cast<EObjectTags>(pLastRulePawn->EastTag));
		}
		break;

	case North:
		// object is attribute
		if (isObject(pFirstRulePawn->NorthTag) && isAttribute(pLastRulePawn->NorthTag))
		{
			ActiveRules.Add(pLastRulePawn->NorthTag, pFirstRulePawn->NorthTag);
			// turn on the light effect in UpdateLightEffect function
			pLastRulePawn->NorthTagActive = true;
			pFirstRulePawn->NorthTagActive = true;
			pIsRulePawn->NorthTagActive = true;
		}
		// object a is object b, change all object a to object b
		else if (isObject(pFirstRulePawn->NorthTag) && isObject(pLastRulePawn->NorthTag))
		{
			ChangeAllEntity(static_cast<EObjectTags>(pFirstRulePawn->NorthTag), static_cast<EObjectTags>(pLastRulePawn->NorthTag));
		}
		break;

	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TryToActivateRule failed: Wrong Face"));
		break;
	}

}

void AGameInfo::UpdateLightEffect() {
	for (TActorIterator<ARulePawn> iter(GetWorld()); iter; ++iter)
	{
		ARulePawn* pRulePawn = *iter;

		UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(pRulePawn->TopMaterial, this);
		UpdateLightEffectForOneFace(DynMaterial, pRulePawn, pRulePawn->TopTagActive, 0);

		// index 1 is bottom face

		DynMaterial = UMaterialInstanceDynamic::Create(pRulePawn->EastMaterial, this);
		UpdateLightEffectForOneFace(DynMaterial, pRulePawn, pRulePawn->EastTagActive, 2);

		DynMaterial = UMaterialInstanceDynamic::Create(pRulePawn->WestMaterial, this);
		UpdateLightEffectForOneFace(DynMaterial, pRulePawn, pRulePawn->WestTagActive, 3);

		DynMaterial = UMaterialInstanceDynamic::Create(pRulePawn->NorthMaterial, this);
		UpdateLightEffectForOneFace(DynMaterial, pRulePawn, pRulePawn->NorthTagActive, 4);

		DynMaterial = UMaterialInstanceDynamic::Create(pRulePawn->SouthMaterial, this);
		UpdateLightEffectForOneFace(DynMaterial, pRulePawn, pRulePawn->SouthTagActive, 5);

	}
}

void AGameInfo::UpdateLightEffectForOneFace(UMaterialInstanceDynamic* DynMaterial, ARulePawn* pRulePawn, bool LightOn, int index) {
	DynMaterial->SetScalarParameterValue("isRuleActive", LightOn);
	pRulePawn->StaticMeshComponent->SetMaterial(index, DynMaterial);
}



bool AGameInfo::WinJudge() const{
	TArray<EObjectTags> WinPawnTags = GetObjectTags(ERuleTags::Win);
	TArray<EObjectTags> YouPawnTags = GetObjectTags(ERuleTags::You);


	for (UnitInfo unitInfo : MapInfo)
	{
		bool bWinL1 = false, bYouL1 = false;
		bool bWinL2 = false, bYouL2 = false;
		for (AParentPawn* pPawn : unitInfo.Objects)
		{
			if (pPawn->onLayer == 1)
			{
				if (WinPawnTags.Find(pPawn->Tag) != INDEX_NONE)
					bWinL1 = true;
				if (YouPawnTags.Find(pPawn->Tag) != INDEX_NONE)
					bYouL1 = true;
			}
			else if (pPawn->onLayer == 2)
			{
				if (WinPawnTags.Find(pPawn->Tag) != INDEX_NONE)
					bWinL2 = true;
				if (YouPawnTags.Find(pPawn->Tag) != INDEX_NONE)
					bYouL2 = true;
			}
 		}
		if ((bWinL1 && bYouL1) || (bWinL2 && bYouL2))
			return true;
	}

	return false;
}

bool AGameInfo::DefeatJudge() const{
	TArray<EObjectTags> DefeatPawnTags = GetObjectTags(ERuleTags::Defeat);
	TArray<EObjectTags> YouPawnTags = GetObjectTags(ERuleTags::You);

	int LivePawnNum = 0;

	for (UnitInfo unitInfo : MapInfo)
	{
		bool bDefeatL1 = false, bYouL1 = false;
		bool bDefeatL2 = false, bYouL2 = false;
		TArray<AParentPawn*> pYouPawnsL1;
		TArray<AParentPawn*> pYouPawnsL2;
		for (AParentPawn* pPawn : unitInfo.Objects)
		{
			if (pPawn->onLayer == 1)
			{
				if (DefeatPawnTags.Find(pPawn->Tag) != INDEX_NONE)
					bDefeatL1 = true;

				if (YouPawnTags.Find(pPawn->Tag) != INDEX_NONE)
				{
					LivePawnNum++;
					pYouPawnsL1.Add(pPawn);
					bYouL1 = true;
				}
			}
			else if (pPawn->onLayer == 2)
			{
				if (DefeatPawnTags.Find(pPawn->Tag) != INDEX_NONE)
					bDefeatL2 = true;

				if (YouPawnTags.Find(pPawn->Tag) != INDEX_NONE)
				{
					LivePawnNum++;
					pYouPawnsL2.Add(pPawn);
					bYouL2 = true;
				}
			}

		}
		if (bDefeatL1 && bYouL1)
		{
			// 销毁这个单元上每一个为You的Pawn
			for (AParentPawn* pDyingPawn : pYouPawnsL1)
			{
				LivePawnNum--;
				pDyingPawn->Destroy();
			}
		}
		if (bDefeatL2 && bYouL2)
		{
			for (AParentPawn* pDyingPawn : pYouPawnsL2)
			{
				LivePawnNum--;
				pDyingPawn->Destroy();
			}
		}
	}

	if (LivePawnNum > 0)
		return false;
	else
		return true;
}

void AGameInfo::ChangeAllEntity(EObjectTags srcTag, EObjectTags destTag) const {
	if (srcTag == destTag)
		return;

	TArray<AEntityPawn*> pSrcPawnArray;
	AExampleEntityPawn* pDestExamplePawn = nullptr;

	for (TActorIterator<AEntityPawn> iter(GetWorld()); iter; ++iter)
	{
		AEntityPawn* pEntityPawn = *iter;
		if (pEntityPawn->Tag == srcTag)
			pSrcPawnArray.Add(pEntityPawn);
	}

	for (TActorIterator<AExampleEntityPawn> iter(GetWorld()); iter; ++iter)
	{
		AExampleEntityPawn* pExampleEntityPawn = *iter;
		if (pExampleEntityPawn->Tag == destTag)
		{
			pDestExamplePawn = pExampleEntityPawn;
			break;
		}
	}

	checkf(pDestExamplePawn, TEXT("Dest object example does not exist"));

	for (AEntityPawn* pSrcPawn: pSrcPawnArray)
	{
		ChangeEntity(pSrcPawn, pDestExamplePawn);
	}
}

void AGameInfo::ChangeEntity(AEntityPawn* pSrcPawn, const AExampleEntityPawn* const pDestPawn) const {
	pSrcPawn->StaticMeshComponent->SetStaticMesh(pDestPawn->StaticMesh);
	pSrcPawn->SkeletalMeshComponent->SetSkeletalMesh(pDestPawn->SkeletalMesh);

	pSrcPawn->haveFace = pDestPawn->haveFace;
	pSrcPawn->bWalkable = pDestPawn->bWalkable;
	pSrcPawn->Tag = pDestPawn->Tag;

	pSrcPawn->haveSkeletaAnimation = pDestPawn->haveSkeletaAnimation;
	pSrcPawn->WalkAnim1 = pDestPawn->WalkAnim1;
	pSrcPawn->WalkAnim2 = pDestPawn->WalkAnim2;
	pSrcPawn->IdleAnim = pDestPawn->IdleAnim;

	pSrcPawn->isSelfRotating = pDestPawn->isSelfRotating;
	pSrcPawn->RotateSpeed = pDestPawn->RotateSpeed;
}
