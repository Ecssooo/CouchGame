#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerBaseState.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractState.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UPlayerInteractState : public UPlayerBaseState
{
	GENERATED_BODY()

public:
	virtual EPlayerStateID GetStateID() const override;

    virtual void OnEnter(UPlayerStateMachine* InSM);
	virtual void OnExit(UPlayerStateMachine* InSM);
};
