#include "Player/PlayerJumpState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/CharacterSettings.h"

void UPlayerJumpState::OnEnter(UPlayerStateMachine* InSM)
{
	const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
	IsJumpLaunched = false;
	IsLeftGround = false;
	if (auto* P = GetPlayer())
	{
		P->GetCharacterMovement()->JumpZVelocity = Settings->JumpPower;
		P->Jump();
		P->ConsumeJump();
		IsJumpLaunched = true;
	}
}

void UPlayerJumpState::OnTick(UPlayerStateMachine* InSM, float)
{
	const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
	if (auto* P = GetPlayer())
	{
		const bool IsOnGround = P->GetCharacterMovement()->IsMovingOnGround();
		const float VZ = P->GetVelocity().Z;

		if (!IsOnGround)
		{
			IsLeftGround = true;
			if (VZ <= 0.f)
			{
				InSM->ChangeState(EPlayerStateID::Fall);
				return;
			}
		}
		else
		{
			if (IsLeftGround && IsJumpLaunched)
			{
				const bool bHasMove = !P->PlayerMoveInput.IsNearlyZero(Settings->MoveDeadZone);
				if (bHasMove && P->IsRunPressed) { InSM->ChangeState(EPlayerStateID::Run); return; }
				if (bHasMove) { InSM->ChangeState(EPlayerStateID::Walk); return; }
				InSM->ChangeState(EPlayerStateID::Idle);
			}
		}
	}
}
