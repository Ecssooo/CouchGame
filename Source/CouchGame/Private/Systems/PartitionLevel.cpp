#include "Systems/PartitionLevel.h"


APartitionLevel::APartitionLevel()
{

	PrimaryActorTick.bCanEverTick = true;
}

void APartitionLevel::UndiscoverSubLevel_Implementation()
{
	
}

void APartitionLevel::DiscoverSubLevel_Implementation()
{
}



void APartitionLevel::BeginPlay()
{
	Super::BeginPlay();
}


void APartitionLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

