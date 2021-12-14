// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleEntityPawn.h"

AExampleEntityPawn::AExampleEntityPawn() {
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = SceneComponent;

	UStaticMeshComponent * StaticMeshComponentJustForEdit = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponentJustForEdit->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone"));
	if (CubeMeshAsset.Succeeded())
	{
		StaticMeshComponentJustForEdit->SetStaticMesh(CubeMeshAsset.Object);
		StaticMeshComponentJustForEdit->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		StaticMeshComponentJustForEdit->SetWorldScale3D(FVector(1.0f));
	}

	StaticMesh = nullptr;
	SkeletalMesh = nullptr;

	SetActorHiddenInGame(true);
}