// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoguelikePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API ARoguelikePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARoguelikePlayerController();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (DisplayName = "Player Main UI Class"))
	TSubclassOf<UUserWidget> PlayerMainUIClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> PlayerMainUI;
};
