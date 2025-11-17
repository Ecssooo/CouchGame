#pragma once
#include "Player/PlayerBaseState.h"
#include "PlayerJumpState.generated.h"

UCLASS(ClassGroup=(States), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UPlayerJumpState : public UPlayerBaseState
{
	GENERATED_BODY()
public:
	virtual EPlayerStateID GetStateID() const override { return EPlayerStateID::Jump; }

protected:
	virtual void OnEnter(UPlayerStateMachine* InSM) override;
	virtual void OnTick(UPlayerStateMachine* InSM, float DeltaTime) override;

private:
	bool IsJumpLaunched = false;
	bool IsLeftGround = false;
};
