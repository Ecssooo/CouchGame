#include "Player/PlayerJumpState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerJumpState::OnEnter(UPlayerStateMachine* InSM)
{
	bJumpLaunched = false;
	bLeftGround = false;
	if (auto* P = GetPlayer())
	{
		P->GetCharacterMovement()->JumpZVelocity = P->JumpPower;
		P->Jump();
		P->ConsumeJump();
		bJumpLaunched = true;
	}
}

void UPlayerJumpState::OnTick(UPlayerStateMachine* InSM, float)
{
	if (auto* P = GetPlayer())
	{
		const bool bOnGround = P->GetCharacterMovement()->IsMovingOnGround();
		const float VZ = P->GetVelocity().Z;

		if (!bOnGround)
		{
			bLeftGround = true;
			if (VZ <= 0.f)
			{
				InSM->ChangeState(EPlayerStateID::Fall);
				return;
			}
		}
		else
		{
			if (bLeftGround && bJumpLaunched)
			{
				const bool bHasMove = !P->PlayerMoveInput.IsNearlyZero(P->MoveDeadZone);
				if (bHasMove && P->IsRunPressed) { InSM->ChangeState(EPlayerStateID::Run); return; }
				if (bHasMove) { InSM->ChangeState(EPlayerStateID::Walk); return; }
				InSM->ChangeState(EPlayerStateID::Idle);
			}
		}
	}
}
