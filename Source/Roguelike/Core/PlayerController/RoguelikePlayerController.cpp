// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerController/RoguelikePlayerController.h"

#include "Blueprint/UserWidget.h"

#include "UI/Widgets/PlayerMainUI.h"
#include "Characters/Player/PlayerCharacter.h"

void ARoguelikePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	if (!PlayerMainUIClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerMainUIClass is not assigned"));

		return;
	}

	PlayerMainUI = CreateWidget<UPlayerMainUI>(
		this,
		PlayerMainUIClass
	);

	if (!PlayerMainUI)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create PlayerMainUI"));

		return;
	}

	PlayerMainUI->AddToViewport();

	TryInitializePlayerUI();
}

void ARoguelikePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TryInitializePlayerUI();
}

void ARoguelikePlayerController::TryInitializePlayerUI()
{
	if (bPlayerUIInitialized)
	{
		return;
	}

	APlayerCharacter* PlayerCharacter =
		Cast<APlayerCharacter>(GetPawn());

	if (!PlayerCharacter || !PlayerMainUI)
	{
		return;
	}

	PlayerMainUI->InitializePlayer(PlayerCharacter);
	bPlayerUIInitialized = true;
}
