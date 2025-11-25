#include "Timeline/Step.h"
#include "Timeline/StepManager.h"

AStep::AStep()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AStep::BeginPlay()
{
	Super::BeginPlay();
}
