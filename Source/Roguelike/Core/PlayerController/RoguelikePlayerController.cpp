// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerController/RoguelikePlayerController.h"

#include "Blueprint/UserWidget.h"

ARoguelikePlayerController::ARoguelikePlayerController()
{
}

void ARoguelikePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	if (!PlayerMainUIClass)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("PlayerMainUIClass is not assigned")
		);

		return;
	}

	PlayerMainUI = CreateWidget<UUserWidget>(
		this,
		PlayerMainUIClass
	);

	if (!PlayerMainUI)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Failed to create PlayerMainUI")
		);

		return;
	}

	PlayerMainUI->AddToViewport();
}
