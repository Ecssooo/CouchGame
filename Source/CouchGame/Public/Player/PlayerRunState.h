#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerRunState.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UPlayerRunState : public UActorComponent
{
	GENERATED_BODY()

public:

	UPlayerRunState();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
