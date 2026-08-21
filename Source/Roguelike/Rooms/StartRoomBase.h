// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/RoomBase.h"
#include "StartRoomBase.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API AStartRoomBase : public ARoomBase
{
	GENERATED_BODY()
	
public:
	AStartRoomBase();

protected:
	virtual bool StartRoom() override;
};
