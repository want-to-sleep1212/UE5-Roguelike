// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthComponent;

UCLASS()
class ROGUELIKE_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UHealthBarComponent();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleHealthChanged(
		float CurrentHealth,
		float MaxHealth,
		float ChangedAmount
	);

private:
	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;
};
