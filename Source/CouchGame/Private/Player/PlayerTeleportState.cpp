#include "Player/PlayerTeleportState.h"



UPlayerTeleportState::UPlayerTeleportState()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}


void UPlayerTeleportState::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void UPlayerTeleportState::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

