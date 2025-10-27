#include "Player/PlayerFallState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerFallState::OnTick(UPlayerStateMachine* InSM, float)
{
	if (auto* P = GetPlayer())
	{
		if (P->GetCharacterMovement()->IsMovingOnGround())
		{
			const bool bHasMove = !P->PlayerMoveInput.IsNearlyZero(P->MoveDeadZone);
			if (bHasMove && P->IsRunPressed) { InSM->ChangeState(EPlayerStateID::Run); return; }
			if (bHasMove) { InSM->ChangeState(EPlayerStateID::Walk); return; }
			InSM->ChangeState(EPlayerStateID::Idle);
		}
	}
}
