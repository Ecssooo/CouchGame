// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Save/SaveInteractionManager.h"

#include "Interactions/Interaction.h"
#include "Systems/Save/SaveCubeSubsystem.h"


// Sets default values
ASaveInteractionManager::ASaveInteractionManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASaveInteractionManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASaveInteractionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASaveInteractionManager::UpdateInteractionDatas(int idInteraction, bool InIsCompleted) const
{
	if (USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>())
	{
		SaveSubsystem->SaveInteractionData(idInteraction, InIsCompleted);
	}
}

void ASaveInteractionManager::UpdateInteractionHighlight()
{
	USaveCubeSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveCubeSubsystem>();
	if (!SaveSubsystem) return;
	
	for (AInteraction* interaction : LevelInteractions)
	{
		UE_LOG(LogTemp, Log, TEXT("Interaction %d is %d."), interaction->InteractionData.InteractionIndex, interaction->InteractionData.IsHighlight);
		interaction->HighlightInteraction(SaveSubsystem->GetInteractionsDatasFromID(interaction->InteractionData.InteractionIndex).IsHighlight);
	}
}

