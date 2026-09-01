// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/ShopRoomBase.h"

#include "UI/Widgets/ShopUI.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/Player/PlayerCharacter.h"

bool AShopRoomBase::StartRoom()
{
	if (!Super::StartRoom())
	{
		return false;
	}

	if (!ShopUIClass)
	{
		return false;
	}

	APlayerController* PlayerController = ShopPlayerController.Get();

	if (!PlayerController)
	{
		return false;
	}

	ShopUI = CreateWidget<UShopUI>(
		PlayerController,
		ShopUIClass
	);

	if (!ShopUI)
	{
		return false;
	}

	ShopUI->OnShopClosed.AddDynamic(
		this,
		&AShopRoomBase::HandleShopClosed
	);

	ShopUI->AddToViewport();

	PlayerController->FlushPressedKeys();
	PlayerController->bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(ShopUI->TakeWidget());

	PlayerController->SetInputMode(InputMode);

	return true;
}

void AShopRoomBase::HandleShopClosed()
{
	if (RoomState != ERoomState::InProgress)
	{
		return;
	}

	if (!ClearRoom())
	{
		return;
	}

	OpenDoors();

	if (ShopUI)
	{
		ShopUI->RemoveFromParent();
		ShopUI = nullptr;
	}

	APlayerController* PlayerController =
		UGameplayStatics::GetPlayerController(this, 0);

	if (!PlayerController)
	{
		return;
	}

	PlayerController->bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
}

void AShopRoomBase::OnRoomTriggerBeginOverlap(
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

	if (!Player)
	{
		return;
	}

	APlayerController* PlayerController =
		Cast<APlayerController>(Player->GetController());

	if (!PlayerController)
	{
		return;
	}

	ShopPlayerController = PlayerController;

	StartRoom();
}
