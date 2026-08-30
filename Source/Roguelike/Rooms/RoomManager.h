// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomManager.generated.h"

class ARoomBase;
class ARoomDoor;

enum class ERoomDirection : uint8
{
	North,
	South,
	East,
	West
};

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	Combat,
	Start,
	Shop,
	Boss
};

UCLASS()
class ROGUELIKE_API ARoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 해당 좌표의 Room Index 반환
	int32 GetRoomIndexAt(int32 X, int32 Y) const;

	// 해당 좌표의 실제 Room Actor 반환
	ARoomBase* GetRoomAt(int32 X, int32 Y) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	int32 GridSize = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	int32 TargetRoomCount = 5;

	UPROPERTY(EditAnywhere, Category = "Room")
	TSubclassOf<ARoomBase> StartRoomClass;

	UPROPERTY(EditAnywhere, Category = "Room")
	TSubclassOf<ARoomBase> CombatRoomClass;

	UPROPERTY(EditAnywhere, Category = "Room")
	TSubclassOf<ARoomBase> ShopRoomClass;

	UPROPERTY(EditAnywhere, Category = "Room")
	TSubclassOf<ARoomBase> BossRoomClass;

	// 양 옆에 있는 벽 -> 너비 1200
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room|Boundary")
	TSubclassOf<AActor> WallClass;

	// 문 대신 들어오는 가운데 벽 -> 너비 600
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room|Boundary")
	TSubclassOf<AActor> CenterWallClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room|Boundary")
	TSubclassOf<ARoomDoor> DoorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	float RoomSpacing = 3000.f;

private:
	void InitializeMap();
	void GenerateMapLayout();
	void AssignRoomTypes();
	void SpawnRooms();

	void CreateRoomBoundaries();

	void CreateBoundary(
		ARoomBase* Room,
		ARoomBase* ConnectedRoom,
		ERoomDirection Direction
	);

	void SpawnWallSegment(
		const FVector& Location,
		const FRotator& Rotation,
		bool bCenterWall
	);

	bool IsValidPosition(int32 X, int32 Y) const;
	bool HasRoomAt(int32 X, int32 Y) const;

	void PrintMap() const;

	int32 FindFarthestRoomIndex() const;
	int32 FindRandomRoomIndexExcept(const TSet<int32>& ExcludedRoomIndices) const;

	FString RoomTypeToString(ERoomType RoomType) const;

	TSubclassOf<ARoomBase> GetRoomClass(ERoomType RoomType) const;

private:
	// Map[Y][X] = Rooms 배열의 Index
	// 빈 칸 = INDEX_NONE
	TArray<TArray<int32>> Map;

	UPROPERTY()
	TArray<TObjectPtr<ARoomBase>> Rooms;

	TArray<ERoomType> RoomTypes;

	int32 GeneratedRoomCount = 0;

	UPROPERTY(EditAnywhere, Category = "Room|Random")
	int32 Seed = 12345;

	UPROPERTY(EditAnywhere, Category = "Room|Random")
	bool bUseRandomSeed = true;

	FRandomStream RandomStream;
};
