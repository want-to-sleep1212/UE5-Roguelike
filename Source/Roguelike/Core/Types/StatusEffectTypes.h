#pragma once

#include "CoreMinimal.h"
#include "StatusEffectTypes.generated.h"

UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
    Invincible,
    Poison
};

USTRUCT(BlueprintType)
struct FStatusEffect
{
    GENERATED_BODY()

    UPROPERTY()
    EStatusEffectType Type;

    UPROPERTY()
    float Duration = 0.f;

    UPROPERTY()
    float ElapsedTime = 0.f;
};