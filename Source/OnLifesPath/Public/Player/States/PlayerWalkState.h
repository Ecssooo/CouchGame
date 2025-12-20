#pragma once

#include "Player/PlayerBaseState.h"
#include "PlayerWalkState.generated.h"

UCLASS(ClassGroup=(States), meta=(BlueprintSpawnableComponent))
class ONLIFESPATH_API UPlayerWalkState : public UPlayerBaseState
{
	GENERATED_BODY()


	
public:
	virtual EPlayerStateID GetStateID() const override { return EPlayerStateID::Walk; }

protected:
	virtual void OnEnter(UPlayerStateMachine* InSM) override;
	virtual void OnTick(UPlayerStateMachine* InSM, float DeltaTime) override;

	//float WalkSmooth = Settings->RunSmoothedMaxSpeed;
};
