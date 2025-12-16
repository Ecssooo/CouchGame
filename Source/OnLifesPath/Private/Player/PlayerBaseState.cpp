#include "Player/PlayerBaseState.h"
#include "Player/PlayerStateMachine.h"
#include "Player/CharacterPlayer.h"

UPlayerBaseState::UPlayerBaseState()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
}

void UPlayerBaseState::EnterState(UPlayerStateMachine* InSM)
{
	SM = InSM;
	SetComponentTickEnabled(true);
	OnEnter(InSM);
}

void UPlayerBaseState::ExitState(UPlayerStateMachine* InSM)
{
	OnExit(InSM);
	SetComponentTickEnabled(false);
	SM.Reset();
}

void UPlayerBaseState::TickState(UPlayerStateMachine* InSM, float DeltaTime)
{
	OnTick(InSM, DeltaTime);
}

ACharacterPlayer* UPlayerBaseState::GetPlayer() const
{
	return SM.IsValid() ? SM->GetPlayer() : nullptr;
}
