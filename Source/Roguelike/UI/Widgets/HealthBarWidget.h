// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class ROGUELIKE_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealth(
		float CurrentHealth,
		float MaxHealth
	);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_Health;
};
