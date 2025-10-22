#include "Player/PlayerGrabState.h"



UPlayerGrabState::UPlayerGrabState()
{

	PrimaryComponentTick.bCanEverTick = true;

}



void UPlayerGrabState::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void UPlayerGrabState::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

