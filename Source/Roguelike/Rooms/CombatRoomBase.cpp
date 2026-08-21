// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/CombatRoomBase.h"

#include "Characters/Enemy/EnemyCharacter.h"

#include "Components/SceneComponent.h"
#include "Engine/World.h"

ACombatRoomBase::ACombatRoomBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACombatRoomBase::RegisterEnemy(AEnemyCharacter* Enemy)
{
	if (!Enemy)
	{
		return;
	}

	AliveEnemies.Add(Enemy);

	Enemy->OnEnemyDead.AddDynamic(
		this,
		&ACombatRoomBase::NotifyEnemyDead
	);
}

void ACombatRoomBase::NotifyEnemyDead(AEnemyCharacter* Enemy)
{
	//UE_LOG(LogTemp, Warning, TEXT("NotifyEnemyDead"));

	if (!Enemy)
	{
		return;
	}

	AliveEnemies.Remove(Enemy);

	if (AliveEnemies.IsEmpty())
	{
		ClearRoom();
	}
}

bool ACombatRoomBase::StartRoom()
{
	if (!Super::StartRoom())
	{
		return false;
	}

	SpawnEnemies();
	CloseDoors();

	return true;
}

bool ACombatRoomBase::ClearRoom()
{
	if (!Super::ClearRoom())
	{
		return false;
	}

	OpenDoors();

	return true;
}

// overriding을 위해 남겨둠
void ACombatRoomBase::SpawnEnemies() {}
