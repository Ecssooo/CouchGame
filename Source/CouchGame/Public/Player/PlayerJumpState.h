#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerJumpState.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UPlayerJumpState : public UActorComponent
{
	GENERATED_BODY()

public:

	UPlayerJumpState();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
