#include "Player/States/PlayerInteractState.h"

#include "Interfaces/Interactable.h"
#include "Player/CharacterPlayer.h"
#include "Player/PlayerStateMachine.h"

EPlayerStateID UPlayerInteractState::GetStateID() const
{
	return EPlayerStateID::Interact;
}

void UPlayerInteractState::OnEnter(UPlayerStateMachine* InSM)
{
	Super::OnEnter(InSM);
	ACharacterPlayer* Player = GetPlayer();
	if (!Player) return;
	AActor* InteractableActor = Player->InteractableActor;
	if (!InteractableActor) return;
	if (!InteractableActor->Implements<UInteractable>()) return;
	
	if(IInteractable::Execute_LockedPlayer(InteractableActor))
	{
		if (!IInteractable::Execute_IsPlayerLocked(InteractableActor))
		{
			IInteractable::Execute_StartInteract(InteractableActor, Player);
			UStaticMeshComponent* socket = IInteractable::Execute_GetPlayerLockParent(InteractableActor);
			if (!socket) return;
			Player->StopVelocity();
			Player->AttachToComponent(socket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			IInteractable::Execute_SetCharacterInInteract(InteractableActor, Player);
			Player->InteractableActor = InteractableActor;
		}else
		{
			Player->StateMachine->ChangeState(EPlayerStateID::Idle);
		}
	}else
	{
		IInteractable::Execute_StartInteract(InteractableActor, Player);
		Player->StateMachine->ChangeState(EPlayerStateID::Idle);
	}
}

void UPlayerInteractState::OnExit(UPlayerStateMachine* InSM)
{
	Super::OnExit(InSM);
	ACharacterPlayer* Player = GetPlayer();
	if (!Player) return;
	AActor* InteractableActor = Player->InteractableActor;
	if (!InteractableActor) return;
	if (!InteractableActor->Implements<UInteractable>()) return;
	
	if (IInteractable::Execute_LockedPlayer(InteractableActor))
	{
		if (!IInteractable::Execute_IsPlayerLocked(InteractableActor))
		{
			IInteractable::Execute_EndInteract(InteractableActor, Player);
			if (Player != IInteractable::Execute_GetCharacterInInteract(InteractableActor)) return;
			Player->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			USceneComponent* unlockTP = IInteractable::Execute_GetUnlockTP(InteractableActor);
			bool PlayerHasMove = Player->SetActorLocation(unlockTP->GetComponentLocation(), false, nullptr, ETeleportType::TeleportPhysics);
			IInteractable::Execute_SetCharacterInInteract(InteractableActor, nullptr);
		}
	}else
	{
		IInteractable::Execute_EndInteract(InteractableActor, Player);
	}
} 
