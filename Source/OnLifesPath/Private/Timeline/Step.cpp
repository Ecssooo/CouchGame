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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Step : [%s] is Start"), *StepName.ToString()));
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
	for (FSubLevel SubLevel : SubLevelsToLock)
	{
		StepManager->SaveCubeManager->UpdateLevelData(SubLevel.FaceID, SubLevel.SubLevelID, false);
	}
	UE_LOG(LogTemp, Log, TEXT("EndStep Implementation"));
	OnEndStep();
}

void AStep::AutoStep()
{
	for (FSubLevel SubLevel : SubLevels)
	{
		StepManager->SaveCubeManager->UpdateLevelData(SubLevel.FaceID, SubLevel.SubLevelID, true);
	}
	OnAutoStep();
}

