#include "Player/PlayerWalkState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerWalkState::OnEnter(UPlayerStateMachine* InSM)
{
	if (auto* P = GetPlayer())
	{
		SmoothedMaxSpeed = P->GetCharacterMovement()->GetLastUpdateVelocity().Size2D();
		P->GetCharacterMovement()->GroundFriction = 2.0f;
		P->GetCharacterMovement()->BrakingFriction = 1.0f;
		P->GetCharacterMovement()->BrakingDecelerationWalking = 700.0f;
	}
}

void UPlayerWalkState::OnTick(UPlayerStateMachine* InSM, float DeltaTime)
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
		if (P->IsRunPressed)
		{
			InSM->ChangeState(EPlayerStateID::Run);
			return;
		}
		const float Target = P->WalkSpeed;
		SmoothedMaxSpeed = FMath::FInterpTo(SmoothedMaxSpeed, Target, DeltaTime, AccelInterpSpeed);
		P->GetCharacterMovement()->MaxWalkSpeed = SmoothedMaxSpeed;
	}
}
