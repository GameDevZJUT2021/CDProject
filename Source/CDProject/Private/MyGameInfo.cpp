 // Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInfo.h"
#include "EntityPawn.h"
#include "Kismet/GameplayStatics.h"
#include "RulePawn.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AMyGameInfo::AMyGameInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundCue> DeadSoundLoader(TEXT("/Game/Music/SoundEffect/dead_Cue"));
	if (DeadSoundLoader.Succeeded())
		DeadSound = DeadSoundLoader.Object;

}

// Init the MapInfo
void AMyGameInfo::Init()
{

	MapInfo.Init(FUnitInfo(), MapLength * MapWidth);

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
void AMyGameInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

 //Called every frame
void AMyGameInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


TArray<EObjectTags> AMyGameInfo::GetObjectTags(ERuleTags ruleTag) const {
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
	

TArray<AEntityPawn*> AMyGameInfo::GetSelfPawns() const {
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

TArray<AEntityPawn*> AMyGameInfo::GetMovePawns() const {
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

void AMyGameInfo::UpdateMapInfo() {
	// ??????????????
	for (int i = 0; i < MapLength * MapWidth; i++)
	{
		MapInfo[i].Objects.Empty();
	}

	// ????????????????
	for (TActorIterator<AParentPawn> iter(GetWorld()); iter; ++iter)
	{
		AParentPawn* pParentPawn = *iter;
		FVector PawnLocation = pParentPawn->GetActorLocation();

		// ??????????????pawn
		if (static_cast<int>(PawnLocation.X) % 100 > 0.01 && static_cast<int>(PawnLocation.Y) % 100 > 0.01)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,TEXT("Location is not aligned, Some errors may occured"));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,pParentPawn->GetHumanReadableName());
			continue;
		}
		// ????????100????
		PawnLocation.X = round(PawnLocation.X);
		PawnLocation.Y = round(PawnLocation.Y);

		pParentPawn->SetActorLocation(PawnLocation);
		
		int x = (PawnLocation.X + (MapWidth - 1) * 50) / 100;
		int y = (PawnLocation.Y + (MapLength - 1) * 50) / 100;
		// ??????????????pawn
		if (0 > x || x >= MapWidth || 0 > y || y >= MapLength)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Location is out of the map, Some errors may occured"));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, pParentPawn->GetHumanReadableName());
			continue;
		}

		if (PawnLocation.Z < Layer1Z + 1 && pParentPawn->onLayer == 2)
			pParentPawn->onLayer = 1;

		else if (PawnLocation.Z > Layer2Z - 1 && pParentPawn->onLayer == 1)
			pParentPawn->onLayer = 2;
		
		MapInfo[x * MapWidth + y].Objects.Push(pParentPawn);
	}
}

bool AMyGameInfo::RuleIsVisible(ECameraAbsLocations CameraAbsLocation, const TArray<int>& MapInfo_X, const TArray<int>& MapInfo_Y, int CurrentIndex) const
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

void AMyGameInfo::UpdateRule(ECameraAbsLocations CameraAbsLocation) {
	ActiveRules.Empty();
	ActiveRulesStore.Empty();
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
			// ??????????????????????????????
			// ????????,????????????????
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
						if (bSouthExist && bNorthExist) // ????????
						{
							TryToActivateRule(pVerticalFirstRulePawn, pVerticalLastRulePawn, pRulePawnArray[i], Face::Top);
							break;
						}
					}
				}
			}
			// ????????
			// ??????3????????????????????????????is, ??????????????????????
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
							// ????south????????????????
							if (pRulePawnArray[i]->SouthTag == ERuleTags::is && RuleIsVisible(ECameraAbsLocations::South, MapInfo_X, MapInfo_Y, i))
								TryToActivateRule(pHorizontalFirstRulePawn, pHorizontalLastRulePawn, pRulePawnArray[i], Face::South);
						}
						else if (CameraAbsLocation == ECameraAbsLocations::North) 
						{
							//????north????????????????
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
			// ????????
			// ????????,????????????????
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

			// ????????
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
							//????West????????????????
							if (pRulePawnArray[i]->WestTag == ERuleTags::is && RuleIsVisible(ECameraAbsLocations::West, MapInfo_X, MapInfo_Y, i))
								TryToActivateRule(pHorizontalFirstRulePawn, pHorizontalLastRulePawn, pRulePawnArray[i], Face::West);
						}
						else if (CameraAbsLocation == ECameraAbsLocations::East)
						{
							//????East????????????????
							if (pRulePawnArray[i]->EastTag == ERuleTags::is && RuleIsVisible(ECameraAbsLocations::East, MapInfo_X, MapInfo_Y, i))
								TryToActivateRule(pHorizontalFirstRulePawn, pHorizontalLastRulePawn, pRulePawnArray[i], Face::East);
						}
						break;
					}
				}
			}
		}
	}
	AllActiveRules = "";
	for (auto Iter(ActiveRulesStore.CreateIterator());Iter;Iter++)
	{
		AllActiveRules += (*Iter + "\n") ;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, AllActiveRules);
	UpdateLightEffect();

}

void AMyGameInfo::TryToActivateRule(ARulePawn* pFirstRulePawn, ARulePawn* pLastRulePawn, ARulePawn* pIsRulePawn, enum Face face) {
	
	switch (face)
	{
	case Top:
		// object is attribute
		if (isObject(pFirstRulePawn->TopTag) && isAttribute(pLastRulePawn->TopTag))
		{
			ActiveRules.Add(pLastRulePawn->TopTag, pFirstRulePawn->TopTag);
			FString lastRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pLastRulePawn->TopTag));
			FString firstRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pFirstRulePawn->TopTag));
			FString activeRule = firstRule + " is " + lastRule;
			ActiveRulesStore.Add(activeRule);
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
			FString lastRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pLastRulePawn->SouthTag));
			FString firstRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pFirstRulePawn->SouthTag));
			FString activeRule = firstRule + " is " + lastRule;
			ActiveRulesStore.Add(activeRule);
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
			FString lastRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pLastRulePawn->WestTag));
			FString firstRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pFirstRulePawn->WestTag));
			FString activeRule = firstRule + " is " + lastRule;
			ActiveRulesStore.Add(activeRule);
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
			FString lastRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pLastRulePawn->EastTag));
			FString firstRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pFirstRulePawn->EastTag));
			FString activeRule = firstRule + " is " + lastRule;
			ActiveRulesStore.Add(activeRule);
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
			FString lastRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pLastRulePawn->NorthTag));
			FString firstRule = StaticEnum<ERuleTags>()->GetNameStringByValue(static_cast<int64>(pFirstRulePawn->NorthTag));
			FString activeRule = firstRule + " is " + lastRule;
			ActiveRulesStore.Add(activeRule);
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

void AMyGameInfo::UpdateLightEffect() {
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

void AMyGameInfo::UpdateLightEffectForOneFace(UMaterialInstanceDynamic* DynMaterial, ARulePawn* pRulePawn, bool LightOn, int index) {
	DynMaterial->SetScalarParameterValue("isRuleActive", LightOn);
	pRulePawn->StaticMeshComponent->SetMaterial(index, DynMaterial);
}



bool AMyGameInfo::WinJudge() const{
	TArray<EObjectTags> WinPawnTags = GetObjectTags(ERuleTags::Win);
	TArray<EObjectTags> YouPawnTags = GetObjectTags(ERuleTags::You);


	for (FUnitInfo unitInfo : MapInfo)
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

bool AMyGameInfo::DefeatJudge() const{
	TArray<EObjectTags> DefeatPawnTags = GetObjectTags(ERuleTags::Defeat);
	TArray<EObjectTags> YouPawnTags = GetObjectTags(ERuleTags::You);

	int LivePawnNum = 0;

	for (FUnitInfo unitInfo : MapInfo)
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
			// ??????????????????????You??Pawn
		   // play dead music effect once
			if (DeadSound)
				UGameplayStatics::PlaySound2D(GetWorld(), DeadSound);
			for (AParentPawn* pDyingPawn : pYouPawnsL1)
			{
				LivePawnNum--;
				pDyingPawn->Destroy();
			}
		}
		if (bDefeatL2 && bYouL2)
		{
			// play dead music effect once
			if (DeadSound)
				UGameplayStatics::PlaySound2D(GetWorld(), DeadSound);
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

void AMyGameInfo::ChangeAllEntity(EObjectTags srcTag, EObjectTags destTag) const {
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

void AMyGameInfo::ChangeEntity(AEntityPawn* pSrcPawn, const AExampleEntityPawn* const pDestPawn) const {
	if (pDestPawn->StaticMesh)
	{
		pSrcPawn->SkeletalMeshComponent->SetSkeletalMesh(nullptr);
		pSrcPawn->StaticMeshComponent->SetStaticMesh(pDestPawn->StaticMesh);
		pSrcPawn->StaticMeshComponent->SetRelativeLocation(pDestPawn->DefaultRelativeLocation);
		pSrcPawn->StaticMeshComponent->SetRelativeRotation(pDestPawn->DefaultRelativeRotation);
		pSrcPawn->StaticMeshComponent->SetRelativeScale3D(pDestPawn->DefaultRelativeScale);
	}
	else if (pDestPawn->SkeletalMesh)
	{
		pSrcPawn->StaticMeshComponent->SetStaticMesh(nullptr);
		pSrcPawn->SkeletalMeshComponent->SetSkeletalMesh(pDestPawn->SkeletalMesh);
		pSrcPawn->SkeletalMeshComponent->SetRelativeLocation(pDestPawn->DefaultRelativeLocation);
		pSrcPawn->SkeletalMeshComponent->SetRelativeRotation(pDestPawn->DefaultRelativeRotation);
		pSrcPawn->SkeletalMeshComponent->SetRelativeScale3D(pDestPawn->DefaultRelativeScale);
	}


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

bool AMyGameInfo::OnLayer2Land(const AParentPawn* pawn) const {
	FVector location = pawn->GetActorLocation();
	return location.X < WestNorthPoint.X&& location.X > EastSouthPoint.X 
		 && location.Y < EastSouthPoint.Y&& location.Y > WestNorthPoint.Y;
}