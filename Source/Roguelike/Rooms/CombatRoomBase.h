// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/RoomBase.h"

class AEnemyCharacter;

#include "CombatRoomBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ROGUELIKE_API ACombatRoomBase : public ARoomBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	virtual void RegisterEnemies();
	virtual void ActivateEnemies();
	virtual void DeactivateEnemies();

	UFUNCTION()
	virtual void NotifyEnemyDead(AEnemyCharacter* Enemy);

	virtual bool StartRoom() override;
	virtual bool ClearRoom() override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<AEnemyCharacter*> PlacedEnemies;

	UPROPERTY()
	TArray<AEnemyCharacter*> AliveEnemies;
};
