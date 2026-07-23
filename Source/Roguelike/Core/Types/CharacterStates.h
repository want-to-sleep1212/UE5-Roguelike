#pragma once

#include "CoreMinimal.h"
#include "CharacterStates.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle,
	Move,
	Dash
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	None,
	Attack,
	Skill
};

UENUM(BlueprintType)
enum class EControlState : uint8
{
	Normal,
	Stunned
};

UENUM(BlueprintType)
enum class ELifeState : uint8
{
	Alive,
	Dead
};
