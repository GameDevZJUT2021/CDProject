// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraDirector.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ACameraDirector::ACameraDirector()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	levelSelect = 1;
    firstInit = 0;
}

// Called when the game starts or when spawned
void ACameraDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    const float SmoothBlendTime = 0.75f;
    //���Ҵ�������ҿ��Ƶ�Actor��
    if (firstInit == 0)
        firstInit++;
    APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if(firstInit==1)
        OurPlayerController->SetViewTarget(CameraOne);
    if (OurPlayerController)
    {
        if (levelSelect == 2 && (OurPlayerController->GetViewTarget() == CameraOne))
        {
            //�����1�л��������2
            OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
        }
        else if (levelSelect == 3 && (OurPlayerController->GetViewTarget() == CameraTwo))
        {
            //�����2�л��������3
            OurPlayerController->SetViewTargetWithBlend(CameraThree, SmoothBlendTime);
        }
        else if (levelSelect == 2 && (OurPlayerController->GetViewTarget() == CameraThree))
        {
            //�����3�л��������2
            OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
        }
        else if (levelSelect == 1 && (OurPlayerController->GetViewTarget() == CameraTwo))
        {
            //�����2�л��������1
            OurPlayerController->SetViewTargetWithBlend(CameraOne, SmoothBlendTime);
        }
    }
}

// Called to bind functionality to input
void ACameraDirector::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    // �ֱ�Ϊ���ҵ��ƶ��¼��󶨺���
    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ACameraDirector::MoveLeft);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ACameraDirector::MoveRight);
}
void ACameraDirector::MoveLeft() {
    if (levelSelect > 1)
        levelSelect--;
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Move Left")));
}
void ACameraDirector::MoveRight() {
    if (levelSelect < 3)
        levelSelect++;
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString(TEXT("Move Right")));
}

