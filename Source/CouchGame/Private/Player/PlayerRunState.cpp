#include "Player/PlayerRunState.h"


UPlayerRunState::UPlayerRunState()
{

	PrimaryComponentTick.bCanEverTick = true;


}



void UPlayerRunState::BeginPlay()
{
	Super::BeginPlay();
	
	
}



void UPlayerRunState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

