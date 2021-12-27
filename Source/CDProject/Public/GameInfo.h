// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParentPawn.h"
#include "EntityPawn.h"
#include "..\CommonEnum.h"
#include "Containers/Map.h"
#include "ExampleEntityPawn.h"
#include "RulePawn.h"
#include "GameInfo.generated.h"

const float Layer1Z = 100.0f;
const float Layer2Z = 500.0f;

struct UnitInfo
{
	UnitInfo() {};
	TArray<AParentPawn*> Objects;
	bool isEmpty() { return !Objects.Num(); }
};



UCLASS()
class CDPROJECT_API AGameInfo : public AActor
{
	GENERATED_BODY()
	
public:
	AGameInfo();

	void Init(int width, int length);

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int test;

	int MapLength;
	int MapWidth;
	TArray	<UnitInfo> MapInfo; // 2D array. UE4 does not support 2D array naturally, so use a simple conversion instead
	TMultiMap<ERuleTags, ERuleTags> ActiveRules;
	TMultiMap<ERuleTags, ERuleTags> RulesPool;

	TArray<EObjectTags> GetObjectTags(ERuleTags ruleTag) const;
	TArray<AEntityPawn*> GetSelfPawns() const;
	TArray<AEntityPawn*> GetMovePawns() const;

	void UpdateMapInfo();
	void UpdateRule(ECameraAbsLocations CameraAbsLocation);
	void UpdateLightEffect();

	bool WinJudge() const;
	bool DefeatJudge() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool RuleIsVisible(ECameraAbsLocations CameraAbsLocation,const TArray<int>& MapInfo_X, const TArray<int> &MapInfo_Y, int CurrentIndex) const;
	enum Face { Top, South, West , East, North};
	void TryToActivateRule(ARulePawn* pFirstRulePawn, ARulePawn* pLastRulePawn, ARulePawn* pIsRulePawn, enum Face face);
	void ChangeAllEntity(EObjectTags srcTag, EObjectTags destTag) const;
	void ChangeEntity(AEntityPawn* pSrcPawn, const AExampleEntityPawn * const pDestPawn) const;
	void UpdateLightEffectForOneFace(UMaterialInstanceDynamic* DynMaterial, ARulePawn* pRulePawn, bool LightOn, int index);
};
