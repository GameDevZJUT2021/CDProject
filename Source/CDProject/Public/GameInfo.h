// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParentPawn.h"
#include "GameInfo.generated.h"

struct UnitInfo
{
	UnitInfo() {};
	TArray<AParentPawn*> Objects;
	TArray<FString> ObjectTypes;
	bool isEmpty() { return !Objects.Num(); }
};



UCLASS()
class CDPROJECT_API AGameInfo : public AActor
{
	GENERATED_BODY()
	
public:
	AGameInfo();

	void Init(int width, int length);

	int MapLength;
	int MapWidth;
	TArray	<UnitInfo> MapInfo; // 2D array. UE4 does not support 2D array naturally, so use a simple conversion instead
	TArray<FString> ActiveRules;
	TArray<FString> RulesPool;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
