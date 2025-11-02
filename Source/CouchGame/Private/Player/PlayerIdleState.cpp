#include "Player/PlayerIdleState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/CharacterSettings.h"


void UPlayerIdleState::OnEnter(UPlayerStateMachine* InSM)
{
    const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
    if (auto* P = GetPlayer())
    {
        P->GetCharacterMovement()->MaxWalkSpeed = Settings->WalkSpeed;
    }
}

void UPlayerIdleState::OnTick(UPlayerStateMachine* InSM, float)
{
    const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
    if (auto* P = GetPlayer())
    {
        if (!P->GetCharacterMovement()->IsMovingOnGround())
        {
            InSM->ChangeState(EPlayerStateID::Fall);
            return;
        }
        const bool bHasMove = !P->PlayerMoveInput.IsNearlyZero(Settings->MoveDeadZone);
        if (bHasMove && P->IsRunPressed) { InSM->ChangeState(EPlayerStateID::Run); return; }
        if (bHasMove) { InSM->ChangeState(EPlayerStateID::Walk); return; }
    }
}
