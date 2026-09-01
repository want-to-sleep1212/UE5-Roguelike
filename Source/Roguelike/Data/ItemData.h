#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Price = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealthIncrease = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDamageIncrease = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
};
