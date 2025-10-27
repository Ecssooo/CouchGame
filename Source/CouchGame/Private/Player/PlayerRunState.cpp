#include "Player/PlayerRunState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerRunState::OnEnter(UPlayerStateMachine* InSM)
{
	if (auto* P = GetPlayer())
	{
		SmoothedMaxSpeed = P->GetCharacterMovement()->GetLastUpdateVelocity().Size2D();
		P->GetCharacterMovement()->GroundFriction = 1.5f;
		P->GetCharacterMovement()->BrakingFriction = 0.8f;
		P->GetCharacterMovement()->BrakingDecelerationWalking = 900.0f;
	}
}

void UPlayerRunState::OnTick(UPlayerStateMachine* InSM, float DeltaTime)
{
	if (auto* P = GetPlayer())
	{
		if (!P->GetCharacterMovement()->IsMovingOnGround())
		{
			InSM->ChangeState(EPlayerStateID::Fall);
			return;
		}
		const bool bHasMove = !P->PlayerMoveInput.IsNearlyZero(P->MoveDeadZone);
		if (!bHasMove)
		{
			InSM->ChangeState(EPlayerStateID::Idle);
			return;
		}
		if (!P->IsRunPressed)
		{
			InSM->ChangeState(EPlayerStateID::Walk);
			return;
		}
		const float Target = P->RunSpeed;
		SmoothedMaxSpeed = FMath::FInterpTo(SmoothedMaxSpeed, Target, DeltaTime, AccelInterpSpeed);
		P->GetCharacterMovement()->MaxWalkSpeed = SmoothedMaxSpeed;
	}
}
