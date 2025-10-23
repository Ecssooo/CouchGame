#include "Player/PlayerInteractState.h"


UPlayerInteractState::UPlayerInteractState()
{

	PrimaryComponentTick.bCanEverTick = true;


}


void UPlayerInteractState::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void UPlayerInteractState::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

