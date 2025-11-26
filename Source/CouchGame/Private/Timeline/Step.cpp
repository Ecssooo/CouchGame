#include "Timeline/Step.h"

#include "Timeline/StepManager.h"
#include "Systems/Save/SaveCubeManager.h"

AStep::AStep()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AStep::BeginPlay()
{
	Super::BeginPlay();
}


void AStep::StartStep()
{
	OnStartStep();
}


void AStep::EndStep()
{
	if (!StepManager || !StepManager->SaveCubeManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndStep: StepManager ou SaveCubeManager est null sur %s"), *GetName());
		return;
	}
	
	for (FSubLevel SubLevel : SubLevels)
	{
		StepManager->SaveCubeManager->UpdateLevelData(SubLevel.FaceID, SubLevel.SubLevelID, true);
	}
	UE_LOG(LogTemp, Log, TEXT("EndStep Implementation"));
	OnEndStep();
}

