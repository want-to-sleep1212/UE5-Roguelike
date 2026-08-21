// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/CombatRoomBase.h"
#include "CombatRoom.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ACombatRoom : public ACombatRoomBase
{
	GENERATED_BODY()
	
protected:
	virtual void SpawnEnemies() override;

protected:
	// 한 방에서 생성할 최소 적 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Spawn")
	int32 MinEnemyCount = 1;

	// 한 방에서 생성할 최대 적 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Spawn")
	int32 MaxEnemyCount = 3;
};
