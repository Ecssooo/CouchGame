// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/PlayerSpawners/MainTeleporterOut.h"


// Sets default values
AMainTeleporterOut::AMainTeleporterOut()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainTeleporterOut::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainTeleporterOut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

