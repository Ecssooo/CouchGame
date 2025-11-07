#include "Player/States/PlayerWalkState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/CharacterSettings.h"


void UPlayerWalkState::OnEnter(UPlayerStateMachine* InSM)
{
	const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
	
	if (auto* P = GetPlayer())
	{
		float WalkSmooth = Settings->RunSmoothedMaxSpeed;
		WalkSmooth = P->GetCharacterMovement()->GetLastUpdateVelocity().Size2D();
		P->GetCharacterMovement()->GroundFriction = Settings->WalkGroundFriction;
		P->GetCharacterMovement()->BrakingFriction = Settings->WalkBrakingFriction;
		P->GetCharacterMovement()->BrakingDecelerationWalking = Settings->WalkBrakingDecelerationWalking;
	}
}

void UPlayerWalkState::OnTick(UPlayerStateMachine* InSM, float DeltaTime)
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

		float WalkSmooth = Settings->WalkSmoothedMaxSpeed;
		const float Target = Settings->WalkSpeed;
		WalkSmooth = FMath::FInterpTo(WalkSmooth, Target, DeltaTime, WalkSmooth);
		P->GetCharacterMovement()->MaxWalkSpeed = WalkSmooth;
		
	}
}
