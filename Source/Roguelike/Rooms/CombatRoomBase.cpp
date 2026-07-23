// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/CombatRoomBase.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Rooms/RoomDoor.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACombatRoomBase::BeginPlay()
{
	Super::BeginPlay();

	RegisterEnemies();
	DeactivateEnemies();
}

void ACombatRoomBase::RegisterEnemies()
{
	for (AEnemyCharacter* Enemy : PlacedEnemies)
	{
		if (Enemy == nullptr)
		{
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("RegisterEnemy"));
		AliveEnemies.Add(Enemy);

		Enemy->OnEnemyDead.AddDynamic(
			this,
			&ACombatRoomBase::NotifyEnemyDead
		);
	}
}

void ACombatRoomBase::ActivateEnemies()
{
	for (AEnemyCharacter* Enemy : AliveEnemies)
	{
		if (Enemy == nullptr)
		{
			continue;
		}

		//UE_LOG(LogTemp, Warning, TEXT("ActivateEnemy: %s Location=%s Hidden=%d Collision=%d Tick=%d"),
		//	*Enemy->GetName(),
		//	*Enemy->GetActorLocation().ToString(),
		//	Enemy->IsHidden(),
		//	Enemy->GetActorEnableCollision(),
		//	Enemy->IsActorTickEnabled()
		//);
		Enemy->Activate();
	}
}

void ACombatRoomBase::DeactivateEnemies()
{
	for (AEnemyCharacter* Enemy : AliveEnemies)
	{
		if (Enemy == nullptr)
		{
			continue;
		}
		//UE_LOG(LogTemp, Warning, TEXT("DeactivateEnemy: %s"), *Enemy->GetName());

		Enemy->Deactivate();
	}
}

void ACombatRoomBase::NotifyEnemyDead(AEnemyCharacter* Enemy)
{
	//UE_LOG(LogTemp, Warning, TEXT("NotifyEnemyDead"));

	if (Enemy == nullptr)
	{
		return;
	}

	AliveEnemies.Remove(Enemy);

	if (AliveEnemies.Num() == 0)
	{
		ClearRoom();
	}
}

bool ACombatRoomBase::StartRoom()
{
	if (Super::StartRoom() == false)
	{
		return false;
	}

	ActivateEnemies();
	CloseDoors();

	return true;
}

bool ACombatRoomBase::ClearRoom()
{
	if (Super::ClearRoom() == false)
	{
		return false;
	}

	OpenDoors();

	return true;
}
