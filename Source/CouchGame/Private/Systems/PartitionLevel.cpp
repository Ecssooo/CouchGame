#include "Systems/PartitionLevel.h"


APartitionLevel::APartitionLevel()
{

	PrimaryActorTick.bCanEverTick = true;
}



void APartitionLevel::BeginPlay()
{
	Super::BeginPlay();
}


void APartitionLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

