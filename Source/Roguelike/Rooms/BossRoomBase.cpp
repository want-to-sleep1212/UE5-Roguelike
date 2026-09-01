// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/BossRoomBase.h"

#include "Characters/Enemy/EnemyCharacter.h"

#include "Components/SceneComponent.h"
#include "Engine/World.h"

#include "Core/RoguelikeGameMode.h"

void ABossRoomBase::SpawnEnemies()
{
	if (BossClasses.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: BossClasses is empty."), *GetName());

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

	const int32 SpawnCount =
		FMath::Min(
			BossClasses.Num(),
			SpawnPoints.Num()
		);

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		if (!BossClasses[i])
		{
			continue;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AEnemyCharacter* SpawnedBoss =
			World->SpawnActor<AEnemyCharacter>(
				BossClasses[i],
				SpawnPoints[i]->GetComponentTransform(),
				SpawnParams
			);

		if (!SpawnedBoss)
		{
			continue;
		}

		RegisterEnemy(SpawnedBoss);
	}
}

bool ABossRoomBase::ClearRoom()
{
	if (!Super::ClearRoom())
	{
		return false;
	}

	if (ARoguelikeGameMode* GameMode =
		Cast<ARoguelikeGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->GameClear();
	}

	return true;
}
