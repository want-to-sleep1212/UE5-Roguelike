// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/RoomBase.h"
#include "Rooms/RoomDoor.h"

#include "Components/BoxComponent.h"

#include "Characters/Player/PlayerCharacter.h"

#define ECC_Player ECC_GameTraceChannel1

// Sets default values
ARoomBase::ARoomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	RoomTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomTrigger"));
	RoomTrigger->SetupAttachment(SceneRoot);

	RoomTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RoomTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	RoomTrigger->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);

	RoomTrigger->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void ARoomBase::BeginPlay()
{
	Super::BeginPlay();

	RoomState = ERoomState::Waiting;

	RoomTrigger->OnComponentBeginOverlap.AddDynamic(
		this,
		&ARoomBase::OnRoomTriggerBeginOverlap
	);
}

//// Called every frame
//void ARoomBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

bool ARoomBase::ClearRoom()
{
	if (RoomState != ERoomState::InProgress)
	{
		return false;
	}

	RoomState = ERoomState::Cleared;

	return true;
}

bool ARoomBase::StartRoom()
{
	if (RoomState != ERoomState::Waiting)
	{
		return false;
	}

	RoomState = ERoomState::InProgress;

	return true;
}

void ARoomBase::RegisterDoor(ARoomDoor* Door)
{
	if (!IsValid(Door))
	{
		return;
	}

	Doors.AddUnique(Door);
}

void ARoomBase::OnRoomTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{

	if (RoomState != ERoomState::Waiting)
	{
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}

	StartRoom();
}

void ARoomBase::OpenDoors()
{
	for (ARoomDoor* Door : Doors)
	{
		if (Door != nullptr)
		{
			Door->OpenDoor();
		}
	}
}

void ARoomBase::CloseDoors()
{
	for (ARoomDoor* Door : Doors)
	{
		if (Door != nullptr)
		{
			Door->CloseDoor();
		}
	}
}
