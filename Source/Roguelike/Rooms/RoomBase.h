// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Core/Types/RoomStates.h"

#include "RoomBase.generated.h"

class ARoomDoor;
class USceneComponent;
class UBoxComponent;

UCLASS(Abstract)
class ROGUELIKE_API ARoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

public:
	virtual bool StartRoom();
	virtual bool ClearRoom();

	void RegisterDoor(ARoomDoor* Door);

protected:
	UFUNCTION()
	void OnRoomTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void CloseDoors();
	void OpenDoors();

protected:

	UPROPERTY(EditAnywhere)
	TArray<ARoomDoor*> Doors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ERoomState RoomState = ERoomState::Waiting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
	TObjectPtr<UBoxComponent> RoomTrigger;
};
