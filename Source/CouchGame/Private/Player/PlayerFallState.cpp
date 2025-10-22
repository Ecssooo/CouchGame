#include "Player/PlayerFallState.h"



UPlayerFallState::UPlayerFallState()
{

	PrimaryComponentTick.bCanEverTick = true;

}



void UPlayerFallState::BeginPlay()
{
	Super::BeginPlay();


}


void UPlayerFallState::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

