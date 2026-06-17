#pragma once

#include "CoreMinimal.h"
#include "RoomStates.generated.h"

UENUM(BlueprintType)
enum class ERoomState : uint8
{
	Waiting,
	InProgress  UMETA(DisplayName = "In Progress"),
	Cleared
};
