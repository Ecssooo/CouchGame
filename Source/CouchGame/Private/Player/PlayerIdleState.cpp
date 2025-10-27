#include "Player/PlayerIdleState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerIdleState::OnEnter(UPlayerStateMachine* InSM)
{
    if (auto* P = GetPlayer())
    {
        P->GetCharacterMovement()->MaxWalkSpeed = P->WalkSpeed;
    }
}

void UPlayerIdleState::OnTick(UPlayerStateMachine* InSM, float)
{
    if (auto* P = GetPlayer())
    {
        if (!P->GetCharacterMovement()->IsMovingOnGround())
        {
            InSM->ChangeState(EPlayerStateID::Fall);
            return;
        }
        const bool bHasMove = !P->PlayerMoveInput.IsNearlyZero(P->MoveDeadZone);
        if (bHasMove && P->IsRunPressed) { InSM->ChangeState(EPlayerStateID::Run); return; }
        if (bHasMove) { InSM->ChangeState(EPlayerStateID::Walk); return; }
    }
}
