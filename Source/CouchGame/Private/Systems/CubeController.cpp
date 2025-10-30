// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CubeController.h"


// Sets default values
ACubeController::ACubeController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACubeController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubeController::AddRotationQuat(AActor* Target, float Pitch, float Yaw, float Roll, bool bLocal)
{
	if (!Target) return;

	// Rotation actuelle
	const FQuat CurrentQuat = Target->GetActorQuat();

	// Crée un quaternion à partir de la rotation demandée (en degrés)
	const FRotator DeltaRot(Pitch, Yaw, Roll);
	const FQuat DeltaQuat = FQuat(DeltaRot);

	FQuat NewQuat;

	if (bLocal)
	{
		// Tourne autour des axes LOCAUX
		NewQuat = DeltaQuat * CurrentQuat;
	}
	else
	{
		// Tourne autour des axes MONDE
		NewQuat = CurrentQuat * DeltaQuat;
	}

	Target->SetActorRotation(NewQuat);
}
