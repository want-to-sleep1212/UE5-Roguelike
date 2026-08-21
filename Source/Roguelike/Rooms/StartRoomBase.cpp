// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/StartRoomBase.h"

#include "Components/BoxComponent.h"

AStartRoomBase::AStartRoomBase()
{
	if (RoomTrigger)
	{
		RoomTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RoomTrigger->SetGenerateOverlapEvents(false);
	}
}

bool AStartRoomBase::StartRoom()
{
	if (!Super::StartRoom())
	{
		return false;
	}

	if (!Super::ClearRoom())
	{
		return false;
	}

	OpenDoors();

	return true;
}
