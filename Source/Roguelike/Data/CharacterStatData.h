#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStatData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;
};
