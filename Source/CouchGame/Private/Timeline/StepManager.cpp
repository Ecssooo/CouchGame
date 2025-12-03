// Fill out your copyright notice in the Description page of Project Settings.


#include "Timeline/StepManager.h"

#include "Kismet/GameplayStatics.h"
#include "Timeline/Step.h"
#include "Systems/Save/SaveCubeManager.h"


// Sets default values
AStepManager::AStepManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStepManager::BeginPlay()
{
	Super::BeginPlay();

	SaveCubeManager = Cast<ASaveCubeManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASaveCubeManager::StaticClass()));

	for (int i = 0; i < ListStep.Num(); i++)
	{
		ListStep[i]->StepManager = this;
	}

	// AutoStep jusqu'au IndexStep
	if (IndexStep >= ListStep.Num() || IndexStep < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Step index out of range"));
	}

	for (int i = 0; i < IndexStep; i++)
	{
		ListStep[i]->AutoStep();
	}
	
	FirstStep();
	
}

void AStepManager::StartStep()
{
	if (!ListStep.IsValidIndex(IndexStep)) return;

	TObjectPtr<AActor> CurrentStep = ListStep[IndexStep];
	if (!CurrentStep) return;

	AStep* Step = Cast<AStep>(CurrentStep);
	if (!Step) return;

	Step->StartStep();
}

void AStepManager::EndStep()
{
	if (!ListStep.IsValidIndex(IndexStep)) return;

	TObjectPtr<AActor> CurrentStep = ListStep[IndexStep];
	if (!CurrentStep) return;

	AStep* Step = Cast<AStep>(CurrentStep);
	if (!Step) return;

	Step->EndStep();

	// TODO : Load les SubLevel et modif la sauvegarde
}

// Called every frame
void AStepManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStepManager::NextStep()
{
	EndStep();
	IndexStep++;
	if (IndexStep >= ListStep.Num()) return;
	StartStep();
}

void AStepManager::FirstStep()
{
	StartStep();
}

void AStepManager::AutoCompleteStep()
{
	ListStep[IndexStep]->AutoStep();
	NextStep();
}

