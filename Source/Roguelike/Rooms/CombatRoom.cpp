// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/CombatRoom.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Algo/RandomShuffle.h"

void ACombatRoom::SpawnEnemies()
{
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: EnemyClass is not set."), *GetName());

		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	TArray<UActorComponent*> SpawnPointComponents =
		GetComponentsByTag(
			USceneComponent::StaticClass(),
			EnemySpawnPointTag
		);

	if (SpawnPointComponents.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: EnemySpawnPoint not found."), *GetName());

		return;
	}

	TArray<USceneComponent*> SpawnPoints;

	for (UActorComponent* Component : SpawnPointComponents)
	{
		USceneComponent* SpawnPoint = Cast<USceneComponent>(Component);

		if (SpawnPoint)
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}

	if (SpawnPoints.IsEmpty())
	{
		return;
	}

	Algo::RandomShuffle(SpawnPoints);

	// BP에 잘못된 값이 들어가더라도 안전하게 보정
	const int32 ValidMinEnemyCount =
		FMath::Clamp(
			MinEnemyCount,
			0,
			SpawnPoints.Num()
		);

	const int32 ValidMaxEnemyCount =
		FMath::Clamp(
			MaxEnemyCount,
			ValidMinEnemyCount,
			SpawnPoints.Num()
		);

	const int32 EnemyCount =
		FMath::RandRange(
			ValidMinEnemyCount,
			ValidMaxEnemyCount
		);

	for (int32 i = 0; i < EnemyCount; ++i)
	{
		USceneComponent* SpawnPoint = SpawnPoints[i];

		if (!SpawnPoint)
		{
			continue;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AEnemyCharacter* SpawnedEnemy =
			World->SpawnActor<AEnemyCharacter>(
				EnemyClass,
				SpawnPoint->GetComponentTransform(),
				SpawnParams
			);

		if (!SpawnedEnemy)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Failed to spawn enemy."), *GetName());

			continue;
		}

		RegisterEnemy(SpawnedEnemy);

		UE_LOG(LogTemp, Log, TEXT("%s: Spawned Enemy %s at %s"), *GetName(), *SpawnedEnemy->GetName(), *SpawnPoint->GetName());
	}

	if (AliveEnemies.IsEmpty())
	{
		ClearRoom();
	}
}
