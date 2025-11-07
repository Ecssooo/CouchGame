#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PartitionLevel.generated.h"

UCLASS(Blueprintable)
class COUCHGAME_API APartitionLevel : public AActor
{
	GENERATED_BODY()

public:

	APartitionLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DiscoverSubLevel();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int IdSublevel;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
};
