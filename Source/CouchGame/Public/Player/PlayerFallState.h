#pragma once
#include "PlayerBaseState.h"
#include "PlayerFallState.generated.h"

UCLASS(ClassGroup=(States), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UPlayerFallState : public UPlayerBaseState
{
	GENERATED_BODY()
public:
	virtual EPlayerStateID GetStateID() const override { return EPlayerStateID::Fall; }

protected:
	virtual void OnTick(UPlayerStateMachine* InSM, float DeltaTime) override;
};
