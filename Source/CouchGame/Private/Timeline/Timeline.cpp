// Fill out your copyright notice in the Description page of Project Settings.


#include "Timeline/Timeline.h"
#include "Timeline/Etape.h"


// Sets default values
ATimeline::ATimeline()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimeline::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimeline::StartEtape()
{
	TObjectPtr<AActor> CurrentEtape = Etapes[IndexEtape];
	if (!CurrentEtape) return;
	if (!CurrentEtape->Implements<IEtape>()) return;

	IEtape::Execute_StartEtape(CurrentEtape);
}

void ATimeline::EndEtape()
{
	TObjectPtr<AActor> CurrentEtape = Etapes[IndexEtape];
	if (!CurrentEtape) return;
	if (!CurrentEtape->Implements<IEtape>()) return;

	IEtape::Execute_EndEtape(CurrentEtape);

	// TODO : Load les SubLevel et modif la sauvegarde
}

// Called every frame
void ATimeline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATimeline::NextEtape()
{
	EndEtape();
	IndexEtape++;
	StartEtape();
}

void ATimeline::FirstEtape()
{
	StartEtape();
}

