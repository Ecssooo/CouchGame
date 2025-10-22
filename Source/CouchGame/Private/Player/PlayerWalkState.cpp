#include "Player/PlayerWalkState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerWalkState::OnEnter(UPlayerStateMachine* InSM)
{
	if (auto* P = GetPlayer())
	{
		//lissage
		SmoothedMaxSpeed = P->GetCharacterMovement()->GetLastUpdateVelocity().Size2D();


		// feeling doux
		P->GetCharacterMovement()->GroundFriction = 2.0f;
		P->GetCharacterMovement()->BrakingFriction = 1.0f;
		P->GetCharacterMovement()->BrakingDecelerationWalking = 700.0f;
	}
}

void UPlayerWalkState::OnTick(UPlayerStateMachine* InSM, float DeltaTime)
{
	if (auto* P = GetPlayer())
	{
		// Priorité
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

		// Walk -> Idle/Run
		constexpr float DeadZone = 0.10f;
		const bool bHasMove = !P->PlayerMoveInput.IsNearlyZero(DeadZone);
		if (!bHasMove)
		{
			InSM->ChangeState(EPlayerStateID::Idle);
			return;
		}
		if (P->bRunPressed)
		{
			InSM->ChangeState(EPlayerStateID::Run);
			return;
		}

		// Lissage
		const float Target = P->WalkSpeed;
		SmoothedMaxSpeed = FMath::FInterpTo(SmoothedMaxSpeed, Target, DeltaTime, AccelInterpSpeed);
		P->GetCharacterMovement()->MaxWalkSpeed = SmoothedMaxSpeed;
	}
}
