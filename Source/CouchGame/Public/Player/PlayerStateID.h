#pragma once
#include "PlayerStateID.generated.h"

UENUM(BlueprintType)
enum class EPlayerStateID : uint8
{
	Idle       UMETA(DisplayName="Idle"),
	Walk       UMETA(DisplayName="Walk"),
	Run        UMETA(DisplayName="Run"),
	Jump       UMETA(DisplayName="Jump"),
	Fall       UMETA(DisplayName="Fall"),
	Interact   UMETA(DisplayName="Interact"),
	Teleport   UMETA(DisplayName="Teleport"),
	Grab       UMETA(DisplayName="Grab")
};
