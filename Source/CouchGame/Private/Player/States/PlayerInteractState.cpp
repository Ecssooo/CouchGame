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
	
	IInteractable::Execute_StartInteract(InteractableActor, Player);
	if(IInteractable::Execute_LockedPlayer(InteractableActor))
	{
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
}

void UPlayerInteractState::OnExit(UPlayerStateMachine* InSM)
{
	Super::OnExit(InSM);
	ACharacterPlayer* Player = GetPlayer();
	if (!Player) return;
	if (!Player->InteractableActor) return;
	if (!Player->InteractableActor->Implements<UInteractable>()) return;
	
	IInteractable::Execute_EndInteract(Player->InteractableActor, Player);
	if (IInteractable::Execute_LockedPlayer(Player->InteractableActor))
	{
		if (Player != IInteractable::Execute_GetCharacterInInteract(Player->InteractableActor)) return;
		
		Player->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Player->SetActorLocation(IInteractable::Execute_GetUnlockTP(Player->InteractableActor)->GetComponentLocation());
		IInteractable::Execute_SetCharacterInInteract(Player->InteractableActor, nullptr);
	}
} 
