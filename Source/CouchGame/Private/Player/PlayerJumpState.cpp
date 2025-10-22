#include "Player/PlayerJumpState.h"



UPlayerJumpState::UPlayerJumpState()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}


void UPlayerJumpState::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void UPlayerJumpState::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

