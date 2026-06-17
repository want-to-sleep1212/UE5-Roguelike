// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/RoomDoor.h"

// Sets default values
ARoomDoor::ARoomDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;
}

//// Called when the game starts or when spawned
//void ARoomDoor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

//// Called every frame
//void ARoomDoor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ARoomDoor::OpenDoor()
{
	if (bIsOpen)
	{
		return;
	}

	bIsOpen = true;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ARoomDoor::CloseDoor()
{
	if (!bIsOpen)
	{
		return;
	}

	bIsOpen = false;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}
