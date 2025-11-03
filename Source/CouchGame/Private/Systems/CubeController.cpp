// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CubeController.h"
#include "LevelStreamerActor.h"
#include "TPManager.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACubeController::ACubeController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TPManager = CreateDefaultSubobject<UTPManager>(TEXT("TPManager"));
}

// Called when the game starts or when spawned
void ACubeController::BeginPlay()
{
	Super::BeginPlay();

	CurrentRotationQuat = FQuat::Identity;
}

// Called every frame
void ACubeController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateStepAxis(DeltaTime);
}

void ACubeController::RotateStepAxis(float DeltaTime)
{
	if (!IsRotating) return;

	Elapsed += DeltaTime;
	float Alpha = Elapsed / AnimationDuration;
	Alpha = FMath::Clamp(Alpha, 0, 1);

	CurrentRotationQuat = FQuat::Slerp(StartQuat, TargetQuat, Alpha);
	SetActorRotation(CurrentRotationQuat);

	if (Elapsed > AnimationDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("Animation termine"));
		IsRotating = false;
		CurrentRotationQuat = TargetQuat;
		SetActorRotation(CurrentRotationQuat);

		TPManager->TrySwitch(CurrentStartLevelDir, CurrentEndLevelDir);
	}
}

void ACubeController::StartRotationQuat(ELevelDir StartDir, ELevelDir EndDir)
{
	IsRotating = true;
	Elapsed = 0.f;

	TPManager->UnloadLevel();

	FQuat DeltaRotationQuat;
	switch (StartDir)
	{
	case ELevelDir::Up:
		DeltaRotationQuat = FQuat::MakeFromEuler(FVector(0.0f, 90.0f, 0.0f));
		break;
	case ELevelDir::Down:
		DeltaRotationQuat = FQuat::MakeFromEuler(FVector(0.0f, -90.0f, 0.0f));
		break;
	case ELevelDir::Left:
		DeltaRotationQuat = FQuat::MakeFromEuler(FVector(90.0f, 0.0f, 0.0f));
		break;
	case ELevelDir::Right:
		DeltaRotationQuat = FQuat::MakeFromEuler(FVector(-90.0f, 0.0f, 0.0f));
		break;
	}

	CurrentStartLevelDir = StartDir;
	CurrentEndLevelDir = EndDir;

	StartQuat = CurrentRotationQuat;
	TargetQuat = DeltaRotationQuat * CurrentRotationQuat;
}
