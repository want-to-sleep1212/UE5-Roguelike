// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/ShopRoomBase.h"

bool AShopRoomBase::StartRoom()
{
	if (!Super::StartRoom())
	{
		return false;
	}

	ClearRoom();
	OpenDoors();

	return true;
}
