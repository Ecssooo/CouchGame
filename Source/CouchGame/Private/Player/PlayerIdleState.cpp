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
        // Priorités
        if (!P->GetCharacterMovement()->IsMovingOnGround())
        {
            InSM->ChangeState(EPlayerStateID::Fall);
            return;
        }

        if (P->bWantsJump)
        {
            P->Jump();
            P->ConsumeJump();
            InSM->ChangeState(EPlayerStateID::Jump);
            return;
        }

        if (P->bWantsInteract)
        {
            P->ConsumeInteract();
            InSM->ChangeState(EPlayerStateID::Interact);
            return;
        }

        // Idle -> Walk/Run
        constexpr float DeadZone = 0.10f;
        const bool bHasMove = !P->PlayerMoveInput.IsNearlyZero(DeadZone);

        if (bHasMove && P->bRunPressed)
        {
            InSM->ChangeState(EPlayerStateID::Run);
            return;
        }

        if (bHasMove)
        {
            InSM->ChangeState(EPlayerStateID::Walk);
            return;
        }
    }
}
