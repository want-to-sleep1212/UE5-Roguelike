// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/RoomBase.h"
#include "ShopRoomBase.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API AShopRoomBase : public ARoomBase
{
	GENERATED_BODY()
	
public:
	virtual bool StartRoom() override;
};
