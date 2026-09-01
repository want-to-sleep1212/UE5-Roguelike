// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/CombatRoomBase.h"
#include "BossRoomBase.generated.h"

class USceneComponent;
class AEnemyCharacter;

UCLASS()
class ROGUELIKE_API ABossRoomBase : public ACombatRoomBase
{
	GENERATED_BODY()

protected:
	virtual void SpawnEnemies() override;

	virtual bool ClearRoom() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss")
	TArray<TSubclassOf<AEnemyCharacter>> BossClasses;
};
