#pragma once
#include "PlayerBaseState.h"
#include "PlayerIdleState.generated.h"

UCLASS(ClassGroup=(States), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UPlayerIdleState : public UPlayerBaseState
{
	GENERATED_BODY()
public:
	virtual EPlayerStateID GetStateID() const override { return EPlayerStateID::Idle; }

protected:
	virtual void OnEnter(UPlayerStateMachine* InSM) override;
	virtual void OnTick(UPlayerStateMachine* InSM, float DeltaTime) override;
};
