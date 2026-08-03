// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/HealthBarWidget.h"

#include "Components/ProgressBar.h"

void UHealthBarWidget::UpdateHealth(
	float CurrentHealth,
	float MaxHealth
)
{
	if (!IsValid(PB_Health))
	{
		return;
	}

	const float HealthPercent =
		MaxHealth > 0.0f
		? CurrentHealth / MaxHealth
		: 0.0f;

	PB_Health->SetPercent(HealthPercent);
}
