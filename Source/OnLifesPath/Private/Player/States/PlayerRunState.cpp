#include "Player/States/PlayerRunState.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/CharacterSettings.h"


void UPlayerRunState::OnEnter(UPlayerStateMachine* InSM)
{
	const UCharacterSettings* Settings = GetDefault<UCharacterSettings>();
	if (auto* P = GetPlayer())
	{
		float RunSmooth = Settings->RunSmoothedMaxSpeed; 
		RunSmooth = P->GetCharacterMovement()->GetLastUpdateVelocity().Size2D();
		P->GetCharacterMovement()->GroundFriction = Settings->RunGroundFriction;
		P->GetCharacterMovement()->BrakingFriction = Settings->RunBrakingFriction;
		P->GetCharacterMovement()->BrakingDecelerationWalking = Settings->RunBrakingDecelerationWalking;
	}
}

void UPlayerRunState::OnTick(UPlayerStateMachine* InSM, float DeltaTime)
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
		if (!P->IsRunPressed)
		{
			InSM->ChangeState(EPlayerStateID::Walk);
			return;
		}
		float RunSmooth = Settings->RunSmoothedMaxSpeed;  
		const float Target = Settings->RunSpeed;
		RunSmooth = FMath::FInterpTo(RunSmooth, Target, DeltaTime, RunSmooth);
		P->GetCharacterMovement()->MaxWalkSpeed = RunSmooth;
	}
}
