// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/RoomBase.h"

#include "CombatRoomBase.generated.h"

class AEnemyCharacter;

/**
 * 
 */
UCLASS(Abstract)
class ROGUELIKE_API ACombatRoomBase : public ARoomBase
{
	GENERATED_BODY()
	
public:
	ACombatRoomBase();

protected:
	virtual void RegisterEnemy(AEnemyCharacter* Enemy);

	UFUNCTION()
	virtual void NotifyEnemyDead(AEnemyCharacter* Enemy);

	virtual bool StartRoom() override;
	virtual bool ClearRoom() override;

	virtual void SpawnEnemies();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<AEnemyCharacter> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	FName EnemySpawnPointTag = TEXT("EnemySpawnPoint");

	UPROPERTY()
	TArray<AEnemyCharacter*> AliveEnemies;
};
