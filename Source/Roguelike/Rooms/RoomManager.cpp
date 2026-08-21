#include "RoomManager.h"
#include "RoomBase.h"
#include "RoomDoor.h"

#include "Containers/Queue.h"

ARoomManager::ARoomManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARoomManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeMap();
	GenerateMapLayout();
	AssignRoomTypes();
	SpawnRooms();
	CreateRoomBoundaries();

	int32 StartRoomIndex = 0;

	Rooms[StartRoomIndex]->StartRoom();
}

void ARoomManager::InitializeMap()
{
	Map.SetNum(GridSize);

	for (TArray<int32>& Row : Map)
	{
		Row.Init(INDEX_NONE, GridSize);
	}
}

void ARoomManager::GenerateMapLayout()
{
	if (GridSize <= 0 || TargetRoomCount <= 0)
	{
		return;
	}

	GeneratedRoomCount = 0;

	const int32 MaxRoomCount = GridSize * GridSize;
	const int32 RoomCount = FMath::Min(TargetRoomCount, MaxRoomCount);

	// 중앙에서 시작
	int32 CurrentX = GridSize / 2;
	int32 CurrentY = GridSize / 2;

	Map[CurrentY][CurrentX] = GeneratedRoomCount++;

	const FIntPoint Directions[] =
	{
		FIntPoint(1, 0),   // North
		FIntPoint(-1, 0),  // South
		FIntPoint(0, 1),   // East
		FIntPoint(0, -1)   // West
	};

	while (GeneratedRoomCount < RoomCount)
	{
		const int32 DirectionIndex = FMath::RandRange(0, 3);

		const int32 NextX = CurrentX + Directions[DirectionIndex].X;

		const int32 NextY = CurrentY + Directions[DirectionIndex].Y;

		if (!IsValidPosition(NextX, NextY))
		{
			continue;
		}

		CurrentX = NextX;
		CurrentY = NextY;

		if (Map[CurrentY][CurrentX] != INDEX_NONE)
		{
			continue;
		}

		Map[CurrentY][CurrentX] = GeneratedRoomCount++;
	}
}

void ARoomManager::AssignRoomTypes()
{
	RoomTypes.Init(ERoomType::Combat, GeneratedRoomCount);

	if (GeneratedRoomCount <= 0)
	{
		return;
	}

	const int32 StartRoomIndex = 0;

	RoomTypes[StartRoomIndex] = ERoomType::Start;

	const int32 BossRoomIndex = FindFarthestRoomIndex();

	if (BossRoomIndex != INDEX_NONE &&
		BossRoomIndex != StartRoomIndex)
	{
		RoomTypes[BossRoomIndex] = ERoomType::Boss;
	}

	TSet<int32> ExcludedRoomIndices;
	ExcludedRoomIndices.Add(StartRoomIndex);
	ExcludedRoomIndices.Add(BossRoomIndex);


	const int32 ShopRoomIndex = FindRandomRoomIndexExcept(ExcludedRoomIndices);

	if (ShopRoomIndex != INDEX_NONE)
	{
		RoomTypes[ShopRoomIndex] = ERoomType::Shop;
	}
}

void ARoomManager::SpawnRooms()
{
	Rooms.SetNum(GeneratedRoomCount);

	for (int32 Y = 0; Y < GridSize; ++Y)
	{
		for (int32 X = 0; X < GridSize; ++X)
		{
			const int32 RoomIndex = Map[Y][X];

			if (RoomIndex == INDEX_NONE)
			{
				continue;
			}

			if (!Rooms.IsValidIndex(RoomIndex))
			{
				UE_LOG(LogTemp, Warning, TEXT("RoomManager: Invalid RoomIndex %d"), RoomIndex);

				continue;
			}

			const ERoomType RoomType = RoomTypes[RoomIndex];

			TSubclassOf<ARoomBase> SelectedRoomClass = GetRoomClass(RoomType);

			if (!SelectedRoomClass)
			{
				UE_LOG(LogTemp, Error, TEXT("Room class is not assigned. RoomIndex: %d, RoomType: %s"), RoomIndex, *RoomTypeToString(RoomType));

				continue;
			}

			const int32 Center = GridSize / 2;

			const int32 RelativeX = X - Center;
			const int32 RelativeY = Y - Center;

			const FVector SpawnLocation =
				GetActorLocation() +
				FVector(
					RelativeX * RoomSpacing,
					RelativeY * RoomSpacing,
					0.1f
				);

			const FRotator SpawnRotation = FRotator::ZeroRotator;

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			ARoomBase* SpawnedRoom =
				GetWorld()->SpawnActor<ARoomBase>(
					SelectedRoomClass,
					SpawnLocation,
					SpawnRotation,
					SpawnParams
				);

			if (!SpawnedRoom)
			{
				UE_LOG(LogTemp, Warning, TEXT("RoomManager: Failed to spawn Room %d at (%d, %d)"), RoomIndex, X, Y);

				continue;
			}

			Rooms[RoomIndex] = SpawnedRoom;
		}
	}
}

void ARoomManager::CreateRoomBoundaries()
{
	for (int32 Y = 0; Y < GridSize; ++Y)
	{
		for (int32 X = 0; X < GridSize; ++X)
		{
			ARoomBase* Room = GetRoomAt(X, Y);

			if (!IsValid(Room))
			{
				continue;
			}



			// North는 항상 현재 Room이 담당
			ARoomBase* NorthRoom = GetRoomAt(X + 1, Y);

			CreateBoundary(
				Room,
				NorthRoom,
				ERoomDirection::North
			);

			// East도 항상 현재 Room이 담당
			ARoomBase* EastRoom = GetRoomAt(X, Y + 1);

			CreateBoundary(
				Room,
				EastRoom,
				ERoomDirection::East
			);

			// South는 다른 Room이 있으면
			// 그 Room의 North가 이미 생성하므로 생략
			if (!HasRoomAt(X - 1, Y))
			{
				CreateBoundary(
					Room,
					nullptr,
					ERoomDirection::South
				);
			}

			// West 역시 다른 Room이 있으면
			// 그 Room의 East가 담당
			if (!HasRoomAt(X, Y - 1))
			{
				CreateBoundary(
					Room,
					nullptr,
					ERoomDirection::West
				);
			}
		}
	}
}

void ARoomManager::CreateBoundary(
	ARoomBase* Room,
	ARoomBase* ConnectedRoom,
	ERoomDirection Direction
)
{
	if (!IsValid(Room))
	{
		return;
	}

	const FVector RoomLocation = Room->GetActorLocation();

	const float HalfRoomSize = RoomSpacing * 0.5f;
	const float SideWallOffset = 900.f;

	FVector BoundaryCenter = RoomLocation;
	FVector SegmentDirection;
	FRotator Rotation = FRotator::ZeroRotator;

	switch (Direction)
	{
	case ERoomDirection::North:

		BoundaryCenter.X += HalfRoomSize;

		// North/South 벽에서는
		// 벽 조각이 Y축 방향으로 나열됨
		SegmentDirection = FVector(0.f, 1.f, 0.f);

		Rotation = FRotator::ZeroRotator;
		break;


	case ERoomDirection::South:

		BoundaryCenter.X -= HalfRoomSize;

		SegmentDirection = FVector(0.f, 1.f, 0.f);

		Rotation = FRotator::ZeroRotator;
		break;


	case ERoomDirection::East:

		BoundaryCenter.Y += HalfRoomSize;

		// East/West 벽에서는
		// 벽 조각이 X축 방향으로 나열됨
		SegmentDirection = FVector(1.f, 0.f, 0.f);

		Rotation = FRotator(0.f, 90.f, 0.f);
		break;


	case ERoomDirection::West:

		BoundaryCenter.Y -= HalfRoomSize;

		SegmentDirection = FVector(1.f, 0.f, 0.f);

		Rotation = FRotator(0.f, 90.f, 0.f);
		break;


	default:
		return;
	}


	const FVector FirstWallLocation =
		BoundaryCenter -
		SegmentDirection * SideWallOffset;

	const FVector SecondWallLocation =
		BoundaryCenter +
		SegmentDirection * SideWallOffset;


	// 양쪽 1200 벽
	SpawnWallSegment(
		FirstWallLocation,
		Rotation,
		false
	);

	SpawnWallSegment(
		SecondWallLocation,
		Rotation,
		false
	);


	// 인접 Room이 있으면 가운데 Door
	if (IsValid(ConnectedRoom))
	{
		if (!DoorClass)
		{
			return;
		}

		ARoomDoor* Door =
			GetWorld()->SpawnActor<ARoomDoor>(
				DoorClass,
				BoundaryCenter,
				Rotation
			);

		if (!IsValid(Door))
		{
			return;
		}

		// 같은 Door 객체를 양쪽 Room에 등록
		Room->RegisterDoor(Door);
		ConnectedRoom->RegisterDoor(Door);
	}
	// 인접 Room이 없으면 가운데 600 Wall
	else
	{
		SpawnWallSegment(
			BoundaryCenter,
			Rotation,
			true
		);
	}
}

void ARoomManager::SpawnWallSegment(
	const FVector& Location,
	const FRotator& Rotation,
	bool bCenterWall
)
{
	TSubclassOf<AActor> ClassToSpawn =
		bCenterWall
		? CenterWallClass
		: WallClass;

	if (!ClassToSpawn)
	{
		return;
	}

	GetWorld()->SpawnActor<AActor>(
		ClassToSpawn,
		Location,
		Rotation
	);
}

void ARoomManager::PrintMap() const
{
	UE_LOG(LogTemp, Log, TEXT("===== Room Layout ====="));

	for (int32 X = GridSize - 1; X >= 0; --X)
	{
		FString RowString;
		for (int32 Y = 0; Y < GridSize; ++Y)
		{
			const int32 RoomIndex = Map[Y][X];

			if (RoomIndex == INDEX_NONE)
			{
				RowString += TEXT(" . ");
			}
			else
			{
				RowString += FString::Printf(TEXT("%2d "), RoomIndex);
			}
		}

		UE_LOG(LogTemp, Log, TEXT("%s"), *RowString);
	}
}

bool ARoomManager::IsValidPosition(int32 X, int32 Y) const
{
	return Map.IsValidIndex(Y) &&
		Map[Y].IsValidIndex(X);
}

bool ARoomManager::HasRoomAt(int32 X, int32 Y) const
{
	if (!IsValidPosition(X, Y))
	{
		return false;
	}

	return Map[Y][X] != INDEX_NONE;
}

int32 ARoomManager::GetRoomIndexAt(int32 X, int32 Y) const
{
	if (!IsValidPosition(X, Y))
	{
		return INDEX_NONE;
	}

	return Map[Y][X];
}

ARoomBase* ARoomManager::GetRoomAt(int32 X, int32 Y) const
{
	if (!HasRoomAt(X, Y))
	{
		return nullptr;
	}

	const int32 RoomIndex = Map[Y][X];

	if (!Rooms.IsValidIndex(RoomIndex))
	{
		return nullptr;
	}

	return Rooms[RoomIndex];
}

int32 ARoomManager::FindFarthestRoomIndex() const
{
	if (GeneratedRoomCount <= 0)
	{
		return INDEX_NONE;
	}

	const int32 StartX = GridSize / 2;
	const int32 StartY = GridSize / 2;

	if (!Map.IsValidIndex(StartY) ||
		!Map[StartY].IsValidIndex(StartX))
	{
		return INDEX_NONE;
	}

	if (Map[StartY][StartX] == INDEX_NONE)
	{
		return INDEX_NONE;
	}

	TArray<TArray<int32>> Distances;
	Distances.SetNum(GridSize);

	for (int32 Y = 0; Y < GridSize; ++Y)
	{
		Distances[Y].Init(-1, GridSize);
	}

	TQueue<FIntPoint> Queue;

	Queue.Enqueue(FIntPoint(StartX, StartY));
	Distances[StartY][StartX] = 0;

	int32 FarthestRoomIndex = Map[StartY][StartX];
	int32 MaxDistance = 0;

	const FIntPoint Directions[] =
	{
		FIntPoint(1, 0),
		FIntPoint(-1, 0),
		FIntPoint(0, 1),
		FIntPoint(0, -1)
	};

	while (!Queue.IsEmpty())
	{
		FIntPoint Current;
		Queue.Dequeue(Current);

		const int32 CurrentDistance = Distances[Current.Y][Current.X];

		const int32 CurrentRoomIndex = Map[Current.Y][Current.X];

		if (CurrentDistance > MaxDistance)
		{
			MaxDistance = CurrentDistance;
			FarthestRoomIndex = CurrentRoomIndex;
		}

		for (const FIntPoint& Direction : Directions)
		{
			const FIntPoint Next = Current + Direction;

			if (Next.X < 0 || Next.X >= GridSize ||
				Next.Y < 0 || Next.Y >= GridSize)
			{
				continue;
			}

			// 방이 없는 칸
			if (Map[Next.Y][Next.X] == INDEX_NONE)
			{
				continue;
			}

			// 이미 방문
			if (Distances[Next.Y][Next.X] != -1)
			{
				continue;
			}

			Distances[Next.Y][Next.X] = CurrentDistance + 1;

			Queue.Enqueue(Next);
		}
	}

	return FarthestRoomIndex;
}

int32 ARoomManager::FindRandomRoomIndexExcept(
	const TSet<int32>& ExcludedRoomIndices) const
{
	TArray<int32> Candidates;

	for (int32 RoomIndex = 0; RoomIndex < RoomTypes.Num(); ++RoomIndex)
	{
		if (ExcludedRoomIndices.Contains(RoomIndex))
		{
			continue;
		}

		Candidates.Add(RoomIndex);
	}

	if (Candidates.IsEmpty())
	{
		return INDEX_NONE;
	}

	const int32 RandomIndex = FMath::RandRange(0, Candidates.Num() - 1);

	return Candidates[RandomIndex];
}

FString ARoomManager::RoomTypeToString(ERoomType RoomType) const
{
	switch (RoomType)
	{
	case ERoomType::Combat:
		return TEXT("Combat");

	case ERoomType::Start:
		return TEXT("Start");

	case ERoomType::Shop:
		return TEXT("Shop");

	case ERoomType::Boss:
		return TEXT("Boss");

	default:
		return TEXT("Unknown");
	}
}

TSubclassOf<ARoomBase> ARoomManager::GetRoomClass(ERoomType RoomType) const
{
	switch (RoomType)
	{
	case ERoomType::Start:
		return StartRoomClass;

	case ERoomType::Combat:
		return CombatRoomClass;

	case ERoomType::Shop:
		return ShopRoomClass;

	case ERoomType::Boss:
		return BossRoomClass;

	default:
		return nullptr;
	}
}
