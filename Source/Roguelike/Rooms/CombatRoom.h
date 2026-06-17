// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rooms/CombatRoomBase.h"
#include "CombatRoom.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ACombatRoom : public ACombatRoomBase
{
	GENERATED_BODY()
	
protected:
	virtual bool StartRoom() override;
};
